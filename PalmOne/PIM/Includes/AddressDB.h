/******************************************************************************
 * Copyright (c) 1995-2005 palmOne, Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/
/**
 * @defgroup PIM PIM Database Record Structures
 *
 * @{
 * @}
 */
/**
 * @ingroup PIM
 */

/**
 * @file AddressDB.h
 *
 * @brief Contains database record type and constants for Contacts application.
 *
 * Contacts application uses a different record format than the old AddressBook
 * application due to some feature enhancement and new data fields. This file
 * contains the structure of the record in Contacts DB and can be used to access
 * the database directly. One way to utilize this header file is to combine it
 * with the old Address Book source code so that the record packing and
 * unpacking routines are adjusted for the new structure.
 *
 * Please note that accessing the database directly is generally not recommended
 * and this is offered as a temporary solution for 3rd party developers. The
 * structure might change again in the future.
 *
 * <hr>
 */

#ifndef ADDRESSDB_H
#define ADDRESSDB_H

#include "AddrDefines.h"

#include <AppLaunchCmd.h>
#include <DataMgr.h>
#include <DateTime.h>

/** Contacts DB version number */
#define addrDBVersionNum        1

#ifndef appErrVersionIncompatible
#define appErrVersionIncompatible   (appErrorClass | 1) /**< Error code */
#endif

/**
 *  Indices for fields member of AddrDBRecordType (record fields)
 **/
typedef enum
{
    name,
    firstName,
    company,
    title,

    phone1,         /**< Phone fields. */
    phone2,
    phone3,
    phone4,
    phone5,
    phone6,
    phone7,

    chat1,          /**< Instant message id and service. */
    chat2,

    webpage,

    custom1,        /**< Custom fields. */
    custom2,
    custom3,
    custom4,
    custom5,
    custom6,
    custom7,
    custom8,
    custom9,

    address,        /**< Address fields. */
    city,
    state,
    zipCode,
    country,
    address2,
    city2,
    state2,
    zipCode2,
    country2,
    address3,
    city3,
    state3,
    zipCode3,
    country3,

    note,           /**< The note field must not be more than ~32K. */

    birthdayDate,   /**< Birthday info. */
    birthdayMask,   /**< Holds AddressDBBirthdayFlags type. */
    birthdayPreset,


    /***************************************************************************
     * All fields starting from picture will be added as blob
     * Each blob is: 4 bytes id, 2 bytes length and data
     * App blobs will have reserved id (1 to 64k) to keep them separate from
     * third party blobs.
     **************************************************************************/

    pictureInfo,    /**< holds picture blob: id --> 1, data  -->  2 bytes
    picture dirty flag, picture data */

    addressFieldsCount

} AddressFields;

#define firstAddressField           name    /**< First index of address field */
#define firstPhoneField             phone1  /**< First index of phone field */
#define lastPhoneField              phone7  /**< Last index of phone field */

/** Total number of phone fields */
#define numPhoneFields              (lastPhoneField - firstPhoneField + 1)

#define firstChatField              chat1   /**< First index of chat field */
#define lastChatField               chat2   /**< Last index of chat field */

#define firstWebField               webpage /**< First index of web field */
#define lastWebField                webpage /**< Last index of web field */

/**
 *  Field label sub-indices relating to the position of the field
 *  label within a popup list of labels.
 *  e.g.: the phone label popup list first selection is "Work", the
 *        second is "Home", etc.
 **/

typedef enum
{
    workLabel,
    homeLabel,
    faxLabel,
    otherLabel,
    emailLabel,
    mainLabel,
    pagerLabel,
    mobileLabel
} AddressPhoneLabels;

/**
 *  Field label sub-indices relating to the position of the field
 *  label within a popup list of labels.
 *  e.g.: the phone label popup list first selection is "Work", the
 *        second is "Home", etc.
 **/

typedef enum
{
    otherChatLabel,
    aimChatLabel,
    msnChatLabel,
    yahooChatLabel,
    icqChatLabel

} AddressChatLabels;

/**
 *  Field label sub-indices relating to the position of the field
 *  label within a popup list of labels.
 *  e.g.: the phone label popup list first selection is "Work", the
 *        second is "Home", etc.
 **/

typedef enum
{
    workAddressLabel,
    homeAddressLabel,
    otherAddressLabel
} AddressStreetAddressLabels;

/** Maximum length of a field label, excluding NULL terminator: */
#define addrLabelLength             16

/** Phone labels: */
#define numPhoneLabelsStoredFirst   numPhoneFields
#define numPhoneLabelsStoredSecond  (numPhoneLabels - numPhoneLabelsStoredFirst)

/** Labels preceding the address field groups (work, home, other): */
/*@{*/
#define Addr1FieldGroupLabel        phone1
#define Addr2FieldGroupLabel        phone2
#define Addr3FieldGroupLabel        phone4
/*@}*/

/** The first and last custom/renameable labels: */
/*@{*/
#define firstRenameableLabel        custom1
#define lastRenameableLabel         custom9
/*@}*/

/** Last label in the first set of labels: */
#define lastLabel                   addrNumDisplayFields

/** Indices to start of second set of phone, address and chat labels: */
/*@{*/
#define phoneLabelSecondStart       addrNumDisplayFields -1
#define chatLabelstart              addrNumDisplayFields
#define webLabelStart               13
/*@}*/

/** Labels for the "Add field" list of fields: */
/*@{*/
#define numAddFieldLabels           3
#define AddFieldLabelStart          addrNumDisplayFields            \
                                    + numPhoneLabelsStoredSecond    \
                                    + numChatLabels
/*@}*/

/** Total field label count: */
/*@{*/
#define numPictureFieldLabels       3
#define numFieldLabels              addrNumDisplayFields            \
                                    + numPhoneLabelsStoredSecond    \
                                    + numChatLabels                 \
                                    + numAddFieldLabels             \
                                    + numPictureFieldLabels
/*@}*/

/***********************************************************************
 *  Application Packed Record Format Related Data
 *
 *  Note: Records are stored in the database in packed format to
 *        conserve space.  When retrieving a record from the database,
 *        it is unpacked into the AddrDBRecordType.  AddrDBGetRecord()
 *        does the necessary unpacking for you.  When creating a new
 *        record, AddrDBNewRecord(), or saving a record, AddrDBChangeRecord(),
 *        the packing is handled for you.
 ***********************************************************************/

/** @brief Packed record flags.
 *
 * AddrDBRecordFlags indicates the address record bits.  It is used
 * to indicate field changes or fields contained in a packed record.
 * Since there is no 64-bit bitfield, a struct with two unsigned
 * longs is used. Please be sure to use the bit macros (defined
 * later below) to extract the bits, as the bit order may change in
 * future.
 */
typedef struct
{
    union
    {
        struct
        {
            UInt32 reserved     :4;

            UInt32 country      :1; /**< Set if record contains a country. */
            UInt32 zipCode      :1; /**< Set if record contains a zipCode. */
            UInt32 state        :1; /**< Set if record contains a state. */
            UInt32 city         :1; /**< Set if record contains a city. */
            UInt32 address      :1; /**< Set if record contains a address. */

            UInt32 custom9      :1; /**< Set if record contains a custom9. */
            UInt32 custom8      :1; /**< Set if record contains a custom8. */
            UInt32 custom7      :1; /**< Set if record contains a custom7. */
            UInt32 custom6      :1; /**< Set if record contains a custom6. */
            UInt32 custom5      :1; /**< Set if record contains a custom5. */
            UInt32 custom4      :1; /**< Set if record contains a custom4. */
            UInt32 custom3      :1; /**< Set if record contains a custom3. */
            UInt32 custom2      :1; /**< Set if record contains a custom2. */
            UInt32 custom1      :1; /**< Set if record contains a custom1. */

            UInt32 webpage      :1; /**< Set if record contains a URL. */

            UInt32 chat2        :1; /**< Set if record contains an instant
                                         message id 2. */
            UInt32 chat1        :1; /**< Set if record contains an instant
                                         message id 1. */

            UInt32 phone7       :1; /**< Set if record contains a phone7. */
            UInt32 phone6       :1; /**< Set if record contains a phone6. */
            UInt32 phone5       :1; /**< Set if record contains a phone5. */
            UInt32 phone4       :1; /**< Set if record contains a phone4. */
            UInt32 phone3       :1; /**< Set if record contains a phone3. */
            UInt32 phone2       :1; /**< Set if record contains a phone2. */
            UInt32 phone1       :1; /**< Set if record contains a phone1. */
            UInt32 title        :1; /**< Set if record contains a title. */
            UInt32 company      :1; /**< Set if record contains a company. */
            UInt32 firstName    :1; /**< Set if record contains a firstName. */
            UInt32 name         :1; /**< Set if record contains a name (bit 0).
            */

        } bits;
        UInt32 allBits;
    };
    union
    {
        struct
        {
            UInt32 reserved2        :6;
            UInt32 reserved         :12;

            UInt32 birthdayPreset   :1; /**< Set if record contains birthday
                                             preset. */
            UInt32 birthdayMask     :1; /**< Set if record contains birthday
                                             mask. */
            UInt32 birthdayDate     :1; /**< Set if record contains birthday
                                             date. */

            UInt32 note             :1; /**< Set if record contains a note
                                             handle. */

            UInt32 country3         :1; /**< Set if record contains a country3.
                                             */
            UInt32 zipCode3         :1; /**< Set if record contains a zipCode3.
                                             */
            UInt32 state3           :1; /**< Set if record contains a state3. */
            UInt32 city3            :1; /**< Set if record contains a city3. */
            UInt32 address3         :1; /**< Set if record contains a address3.
                                             */

            UInt32 country2         :1; /**< Set if record contains a country2.
                                             */
            UInt32 zipCode2         :1; /**< Set if record contains a zipCode2.
                                             */
            UInt32 state2           :1; /**< Set if record contains a state2. */
            UInt32 city2            :1; /**< Set if record contains a city2. */
            UInt32 address2         :1; /**< Set if record contains a address2.
                                             */


        } bits2;
        UInt32 allBits2;
    };
} AddrDBRecordFlags;

/***********************************************************************
 *  Application Unpacked Record Format Related Data
 ***********************************************************************/

/** @brief Label types.
 *
 * This describes the label types for those user selectable label
 * fields: phone, instant messenger, and address.
 */
typedef struct
{
    union
    {
        struct
        {
            unsigned displayPhoneForList:4; /**< The phone displayed for the
                                                 List View, corresponding to the
                                                 item in the AddressPhoneLabels
                                                 enum. */
            unsigned phone7:4;              /**< Which phone label (work, home,
                                                 fax, other, ...) for this phone
                                                 field, value is an
                                                 AddressPhoneLabels enum. */
            unsigned phone6:4;
            unsigned phone5:4;
            unsigned phone4:4;
            unsigned phone3:4;
            unsigned phone2:4;
            unsigned phone1:4;
        } phones;
        UInt32 phoneBits;
    };

    union
    {
        struct
        {
            unsigned int reserved : 4;
            unsigned int address3 : 4;  /**< Which address label (work, home,
                                             other) for this address field,
                                             value is an
                                             AddressStreetAddressLabels enum. */
            unsigned int address2 : 4;
            unsigned int address1 : 4;
        }addresses;
        UInt16 addressBits;
    };

    union
    {
        struct
        {
            unsigned int reserved:8;
            unsigned int chat2:4;       /**< Which chat label (other, aim, msn,
                                          ...) for this chat field, value is an
                                             AddressChatLabels enum. */
            unsigned int chat1:4;

        }chatIds;
        UInt16 chatBits;
    };

} AddrOptionsType;

/** @brief Birthday data flags
 *
 * On/off flags for birthday features.
 */
typedef struct
{
    unsigned short reserved     :15;
    unsigned short alarm        :1; /**< Indicates if a reminder is set for this
                                         birthday. */
}
AddressDBBirthdayFlags;

/** @brief Birthday format
 *
 * This is the format of the birthday data in the record.
 */
typedef struct
{
    DateType                birthdayDate;   /**< Contains birthdate. Year should
                                                 be considered if year mask it
                                                 set. */
    AddressDBBirthdayFlags  birthdayMask;
    UInt8                   birthdayPreset; /**< Number of days for advance
                                                 reminder. */
} BirthdayInfo;

/** @brief Picture info
 *
 * AddressDBPictureInfo is a temp struct to hold the picture data.Within the
 * address record picture is stored as blob data.
 */
typedef struct
{
    UInt16  pictureDirty;   /**< set when a picture is edited or added. */
    UInt16  pictureSize;    /**< size of the image in bytes */
    void*   pictureData;    /**< image data in jpeg format, limit is 4k */
} AddressDBPictureInfo;

/** Total number of blobs a record is allowed to have */
#define apptMaxBlobs                    10

/** @brief Format of each blob attached to a record. */
typedef struct
{
    UInt32                  creatorID;
    UInt16                  size;       /**< Excludes space used by blob ID
                                             and size. */
    void *                  content;    /**< Remainder of blob is content */
} BlobType;

typedef BlobType * BlobPtr;



///////////////////////////JJJJJ//////////////////////////////

typedef struct
{
	UInt16					dirty;
	BirthdayInfo			anniversaryInfo;
	ToneIdentifier			ringtoneInfo;
} Release2BlobType;

//////////////////////////JJJJJJ///////////////////////////////



/** @brief Record format
 *
 * This is the unpacked record format as used by the app.  Pointers are
 * either NULL or point to strings elsewhere on the card.  All strings
 * are NULL character terminated.
 */
typedef struct
{
    /** Display by company or by name */
    AddrOptionsType         options;

    /** Container for all fields of type string */
    Char *                  fields[addrNumStringFields];

    /** Container to hold birthday information */
    BirthdayInfo            birthdayInfo;

    /**
     * Container to hold picture information: dirty flags, followed
     * by size and the actual image */
    AddressDBPictureInfo    pictureInfo;
    
    
    
    
    
    /******* JJJJJJJ**********/
    Release2BlobType		rel2blobInfo;
    
    /******** JJJJJJJ *******/
    
    
    
    
    

    /** Number of blobs the record has excluding the picture blob */
    UInt16                  numBlobs;

    /** Container to hold blobs and the metadata */
    BlobType                blobs[apptMaxBlobs];

} AddrDBRecordType;

typedef AddrDBRecordType *AddrDBRecordPtr;

/***********************************************************************
 *  Contacts Application Info Block Related Data
 ***********************************************************************/

/** Record field label string. */
typedef char addressLabel[addrLabelLength];

/** @brief Field label state
 *
 * This is used to indicate changes to record field labels. e.g.:
 * Flags are set when copying the localized field labels into the
 * fieldLabels array of the AddrAppInfoType.
 */
typedef struct
{
    union
    {
        struct
        {
            UInt32 reserved     :4;

            UInt32 country      :1; /**< Set if country label is dirty. */
            UInt32 zipCode      :1; /**< Set if zipcode label is dirty. */
            UInt32 state        :1; /**< Set if state label is dirty. */
            UInt32 city         :1; /**< Set if city label is dirty. */
            UInt32 address      :1; /**< Set if address label is dirty. */

            UInt32 custom9      :1; /**< Set if custom/renameable label is
            dirty. */
            UInt32 custom8      :1;
            UInt32 custom7      :1;
            UInt32 custom6      :1;
            UInt32 custom5      :1;
            UInt32 custom4      :1;
            UInt32 custom3      :1;
            UInt32 custom2      :1;
            UInt32 custom1      :1;

            UInt32 webpage      :1; /**< Set if webpage label is dirty. */

            UInt32 chat2        :1; /**< Set if instant message id is dirty. */
            UInt32 chat1        :1;

            UInt32 phone7       :1; /**< Set if phone label is dirty. */
            UInt32 phone6       :1;
            UInt32 phone5       :1;
            UInt32 phone4       :1;
            UInt32 phone3       :1;
            UInt32 phone2       :1;
            UInt32 phone1       :1;
            UInt32 title        :1; /**< Set if title label is dirty. */
            UInt32 company      :1; /**< Set if company label is dirty. */
            UInt32 firstName    :1; /**< Set if firstName label is dirty. */
            UInt32 name         :1; /**< Set if name label is dirty (bit 0). */

        } bits;
        UInt32 allBits;
    };
    union
    {
        struct
        {
            UInt32 reserved1        :12;
            UInt32 reserved2        :7;

            UInt32 pictureInfo      :1; /**< Set if picture label is dirty. */

            UInt32 birthdayDate     :1; /**< Set if birthday label is dirty. */

            UInt32 note             :1; /**< Set if note field is dirty. */

            UInt32 country3         :1; /**< Set if address fields are dirty. */
            UInt32 zipCode3         :1;
            UInt32 state3           :1;
            UInt32 city3            :1;
            UInt32 address3         :1;

            UInt32 country2         :1;
            UInt32 zipCode2         :1;
            UInt32 state2           :1;
            UInt32 city2            :1;
            UInt32 address2         :1;

        } bits2;
        UInt32 allBits2;
    };
} AddrDBFieldLabelsDirtyFlags;

/** @brief Miscellaneous application data
 *
 * This includes any miscellaneous application wide data.
 */
typedef struct
{
    unsigned reserved:7;
    unsigned sortByCompany  :1;
} AddrDBMisc;

/** @brief Application Info Block
 *
 * This structure is used to store info applicable to all records
 * in the database, specific to the application, inter-session (like
 * preferences), etc.
 *
 * Note: The phone field labels are stored specially.  Each phone
 *       field can use one of eight labels.  Some of those eight
 *       labels are stored as the phone field labels.  The remainder
 *       (phoneLabelsStoredAtEnd) are stored after the labels for
 *       all the fields.
 */
typedef struct
{
    /** Bitfield of categories with a different name. */
    UInt16 renamedCategories;

    char   categoryLabels[dmRecNumCategories][dmCategoryLength];

    UInt8  categoryUniqIDs[dmRecNumCategories];

    /**
     * Unique IDs generated by the device are between 0 - 127.
     * Those from the PC are 128 - 255.
     */
    UInt8  lastUniqID;

    /** From the compiler word aligning things. */
    UInt8  reserved1;

    /**
     * Whether category colors were edited since last sync.
     * Least significant bit first.
     */
    UInt16 categoryColorsEdited;

    /**
     * Added as part of the Mullet version of this application,
     * so that we can later support color categories without
     * breaking the conduits.
     */
    UInt8  categoryColors[dmRecNumCategories];

    UInt16 reserved2;

    /** Labels that have changed. */
    AddrDBFieldLabelsDirtyFlags dirtyFieldLabels;

    /** Labels displayed for record fields. */
    addressLabel                fieldLabels[numFieldLabels];

    /** Country for which the database (labels) is formatted. */
    CountryType                 country;

    UInt8                       reserved;

    AddrDBMisc                  misc;

} AddrAppInfoType;

typedef AddrAppInfoType *AddrAppInfoPtr;

/** Macro to determine whether a lookup field is for phone lookup */
#define IsPhoneLookupField(p)   (addrLookupWork <= (p) && (p) <= \
                                 addrLookupMobile)

#define firstRecordFlagBits2Field   address2

/**
 * Get a bit mask for the given bit position.
 *
 * The pos argument is used to indicate what bit position
 * should be set in the 4-byte bit mask, so a pos = 2 yields:
 * 0000 0000 0000 0010
 *
 * @param pos   Position of the bit to be set in the 4 byte
 *                  bit mask.
 * @return      A 4-byte bit mask.
 */
#define BitAtPosition(pos)  ((UInt32)1 <<                               \
                            ((pos < firstRecordFlagBits2Field) ?        \
                              pos:(pos - firstRecordFlagBits2Field)) )

/**
 * Get the record flag value for the given flag index.
 *
 * @param recFlags  A AddrDBRecordFlags struct.
 * @param index     The flag index. i.e.: the flag bit position.
 *
 * @return          The value of the record flag for the given flag index.
 */
#define GetBitMacro(recFlags, index)    ((index < firstRecordFlagBits2Field) ? \
                                        ((recFlags).allBits) & BitAtPosition(  \
                                          index):((recFlags).allBits2) &       \
                                          BitAtPosition(index))

/**
 * Set the record flag value at the given flag index.
 *
 * @param recFlags  A AddrDBRecordFlags struct.
 * @param index     The index of the flag to set. i.e.:
 *                      the bit position of the flag to set.
 *
 * @return          Nothing.
 */
#define SetBitMacro(recFlags, index)    if(index < firstRecordFlagBits2Field)\
                                          (recFlags).allBits |= BitAtPosition(index);  else \
                                          (recFlags).allBits2 |=  BitAtPosition(index)

/**
 * Clear the record flag value at the given flag index.
 * This clears or sets the flag value to zero (0).
 *
 * @param recFlags  A AddrDBRecordFlags struct.
 * @param index     The index of the flag to clear. i.e.:
 *                      the bit position of the flag to set.
 *
 * @retval          Nothing.
 */
#define RemoveBitMacro(recFlags, index) if(index < firstRecordFlagBits2Field)  \
                                        ((recFlags).allBits) &= ~BitAtPosition \
                                        (index); else                          \
                                        ((recFlags).allBits2) &=               \
                                        ~BitAtPosition(index)

#define SetBitMacroBits(bitField, index) (bitField |= BitAtPosition(index))

#define PictureFieldListStart   addrNumDisplayFields +          \
                                numPhoneLabelsStoredSecond +    \
                                numChatLabels +                 \
                                numAddFieldLabels

#define phoneLabelNone          -1

#define noPhoneField            0x0000

// <chg 20-sep-99 dia> These are phone fields that you might want to make
// a voice phone call to...
#define IsVoiceLabel(f) (((f) != P1ctFaxPhone) && ((f) != P1ctEmailPhone))

#define IsPhoneLookupField(p) (addrLookupWork <= (p) && (p) <= addrLookupMobile)

#define IsSMSLabel(f) (((f) == P1ctMobilePhone) || ((f) ==P1ctEmailPhone) ||   \
                       ((f) == P1ctOtherPhone))

#define IsSMSLabelNoEmail(f) (((f) == P1ctMobilePhone) ||   \
                              ((f) == P1ctOtherPhone))

/******************************************************************************
 * BLOB ID definition
 * Internal blob id will start from 'Bd00'.
 * Creator ids from 'Bd00 - 'Bd09' is registered for this purpose.
 ******************************************************************************/

/** picture blob id */
#define addrPictureBlobId                       'Bd00'
#define addrRelease2BlobId						'Bd01'    // Release 2

// Need this constant to hack the fix for a Mullet Outlook conduit bug, which is
// already shipped.
#define appInfoBlockSizeMullet  1092

#define kCallerIDMaxPhoneNumberDigits           63

// This is the number of digits in a phone number that are significant...
// Only check the last 8 chars to allow number to be stored in international
// format to still match if bufferLen is greater than key len just use the key
// length
#define kCallerIDSignificantPhoneNumberDigits   8

#define kStrLenRequiredForMatch                 6

// This structure is only for the exchange of address records.
typedef union
{
    struct
    {
        unsigned reserved       :8;

        // Typically only one of these are set

        unsigned email          :1; /**< set if data is an email address */
        unsigned fax            :1; /**< set if data is a fax */
        unsigned pager          :1; /**< set if data is a pager */
        unsigned voice          :1; /**< set if data is a phone */

        unsigned mobile         :1; /**< set if data is a mobile phone */

        // These are set in addition to other flags.

        unsigned work           :1; /**< set if phone is at work */
        unsigned home           :1; /**< set if phone is at home */

        // Set if this number is preferred over others.  May be preferred
        // over all others.  May be preferred over other emails.  One
        // preferred number should be listed next to the person's name.

        unsigned preferred      :1; /**< set if this phone is preferred (bit 0)
                                         */
    } bits;
    UInt32 allBits;
} AddrDBPhoneFlags;

typedef struct
{
    UInt16          currentCategory;
    FontID          v20NoteFont;                // For 2.0 compatibility (BGT)
    Boolean         showAllCategories;
    Boolean         saveBackup;
    Boolean         rememberLastCategory;

    // Version 3 preferences
    FontID          addrListFont;
    FontID          addrRecordFont;
    FontID          addrEditFont;
    UInt8           reserved1;
    UInt32          businessCardRecordID;
    FontID          noteFont;
    UInt8           reserved2;

    // Version 4 preferences
    Boolean         enableTapToConnect;

    // Version 5 preferences for wireless products
    Boolean         showSimCategory;

} AddrPreferenceType;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get ptr to the application database info block.
 *
 * @param dbP   Open database ptr.
 * @return      Locked ptr to the AddrAppInfo or NULL.
 */
extern AddrAppInfoPtr AddrDBAppInfoGetPtr (DmOpenRef dbP);

/**
 * Create an app info chunk if missing and initialize with defaults.
 *
 * @param dbP   Open database ptr.
 * @return      0 if successful, error code if not.
 */
extern Err AddrDBAppInfoInit (DmOpenRef dbP);

/**
 * Set the field labels to those appropriate to the current
 * country (based on system preferences).
 *
 * @param appInfoP  Application info block ptr.
 * @return          Nothing.
 */
extern void AddrDBChangeCountry (AddrAppInfoPtr appInfoP);

/**
 * Set a record field's label and mark it dirty.
 *
 * @param dbP           Open database ptr.
 * @param fieldNum      Field label to change.
 * @param fieldLabel    New field label to use.
 * @return              Nothing.
 */
extern void AddrDBSetFieldLabel (DmOpenRef dbP, UInt16 fieldNum,
                                 Char * fieldLabel);

/**
 * Return the scheduled alarm time (in seconds) associated with the given
 * record.
 *
 * @param record         Ptr to the record from which to retrieve the alarm
 *                           time.
 * @param alarmSeconds   Ptr to var to set with the alarm time in seconds
 *                           from 1/1/1904.
 * @param wrapToNextYear True if the alarm time returned should be for next
 *                           year when the alarm time is before the current
 *                           time.
 * @return               True if the alarm is on (set to occur).
 */
extern Boolean AddrDBGetAlarm (AddrDBRecordType* record, UInt32 *alarmSeconds,
                               Boolean wrapToNextYear);

/**
 * Return the birthdate (in seconds) associated with the given record.
 *
 * @param record        Ptr to the record from which to retrieve the
 *                          birthdate.
 * @return              Birthdate in seconds from 1/1/1904.
 */
extern UInt32 AddrDBGetBirthday (AddrDBRecordType* record);

/**
 * Get the application's database, opening or creating it as neccessary.
 *
 * @param addrPP    Ptr to a database ref var (DmOpenRef) to set.
 * @param mode      How to open the database (e.g.: dmModeReadWrite).
 * @return          0 if successful, error code if not.
 */
extern Err AddrDBGetDatabase (DmOpenRef *addrPP, UInt16 mode);

/**
 * Change a record in the database.
 *
 * @param dbP           Open database ptr.
 * @param index         Index of database record to change.
 * @param r             Ptr to a record having the changes to save.
 * @param changedFields The fields to change.
 * @param blobDirty     True if data in the blob has been changed
 * @return              0 if successful, error code if not.
 */
Err AddrDBChangeRecord(DmOpenRef dbP, UInt16 *index, AddrDBRecordPtr r,
                       AddrDBRecordFlags changedFields, UInt16 BlobDirty);

/**
 * Get a record's data from the database and lock the record.
 *
 * @param dbP       Open database ptr.
 * @param index     Index of database record to retrieve.
 * @param recordP   Ptr to a record to initialize with the retrieved data.
 * @param recordH   Record handle to unlock when finished with the record.
 * @return          0 if successful, error code if not.
 */
extern Err AddrDBGetRecord (DmOpenRef dbP, UInt16 index,
                            AddrDBRecordPtr recordP, MemHandle *recordH);

/**
 * Create a new packed record in sorted position in the database.
 *
 * @param dbP   Open database ptr.
 * @param r     Ptr to a record to copy into the DB.
 * @param index Ptr to index var to set with the new record's index.
 * @return      0 if successful, error code if not.
 */
extern Err AddrDBNewRecord (DmOpenRef dbP, AddrDBRecordPtr r, UInt16 *index);

/**
 * Checks if a record is empty or contains data.
 *
 * @param recordP   Ptr to a record to examine.
 * @return          true if the record contains data, false if it is empty.
 */
extern Boolean AddrDBRecordContainsData (AddrDBRecordPtr recordP);

/**
 * Change the sort order of the database.
 *
 * @param dbP               Open database ptr.
 * @param sortByCompany     True if sort order should be by the company field.
 * @return                  Always returns 0.
 */
extern Err AddrDBChangeSortOrder (DmOpenRef dbP, Boolean sortByCompany);

/**
 * Find the record containing the most of the string passed.  Searches all
 * categories. Calls AddrDBLookupString() and AddrDBLookupSeekRecord().
 *
 * @param dbP               Open database ptr.
 * @param key               String for which to search.
 * @param sortByCompany     True if database sort order is by company field.
 * @param field1            Field that the sought record should contain.
 * @param field2            Field that the sought record should contain.
 * @param recordP           Ptr to record index var to set with the index
 *                              of the found record.
 * @param phoneP            Ptr to a var to set with the phone field index
 *                              having
 *                           data in a found record.
 * @param lookupFieldMap    Array mapping look-up fields to record fields, where
 *                              the array index is a look-up field number, and
 *                              the data in that element is the corresponding
 *                              record field number.
 * @param completeMatch     True if a record contains all of the given key.
 * @param uniqueMatch       True if the found record is the only record with the
 *                              key matching to the extent the found record
 *                              does.
 *
 * @return True if a record is found; false if no string is passed or there
 *             aren't any records.
 */
extern Boolean AddrDBLookupLookupString (DmOpenRef dbP, Char *key,
                                         Boolean sortByCompany,
                                         AddressLookupFields field1,
                                         AddressLookupFields field2,
                                         UInt16 *recordP, Int16 *phoneP,
                                         AddressFields lookupFieldMap[],
                                         Boolean * completeMatch,
                                         Boolean *uniqueMatch);

/**
 * Given the index of a record, scan forewards or backwards for a displayable
 * record having the given fields.
 *
 * @param dbP               Open database ptr.
 * @param indexP            Ptr to the index of a record to start from; the
 *                              index of the record found is returned in this
 *                              parameter.
 * @param phoneP            Ptr to a var to set with the phone field index
 *                              having data in a found record.
 * @param offset            Number of records to skip:
 *                              0  - seek from the current record to the next
 *                                   displayable record, if the current record
 *                                   is displayable, its index is returned.
 *                              1  - seek foreward, skipping one displayable
 *                                   record.
 *                             -1  - seek backwards, skipping one displayable
 *                                   record.
 * @param direction         Direction to seek: dmSeekForward or dmSeekBackward.
 * @param field1            Field that the sought record should contain.
 * @param field2            Field that the sought record should contain.
 * @param lookupFieldMap    Array mapping look-up fields to record fields, where
 *                              the array index is a look-up field number, and
 *                              the data in that element is the corresponding
 *                              record field number.
 * @return                  True if a displayable record was found.
 */
extern Boolean AddrDBLookupSeekRecord (DmOpenRef dbP, UInt16 * indexP,
                                       Int16 *phoneP, Int16 offset,
                                       Int16 direction,
                                       AddressLookupFields field1,
                                       AddressLookupFields field2,
                                       AddressFields lookupFieldMap[]);

/**
 * Find the record containing the most of the string passed. Called by
 * AddrDBLookupLookupString().
 *
 * @param dbP                   Open database ptr.
 * @param key                   String for which to search.
 * @param sortByCompany         True if database sort order is by company field.
 * @param category              What category to restrict the search.  Pass
 *                                  dmAllCategories for no restriction to
 *                                  records in a specific category.
 * @param recordP               Ptr to record index var to set with the index
 *                                  of the found record.
 * @param completeMatch         True if a record contains all of the given key.
 * @param oNumMatchingCharsP    Ptr to the var to set with the number of chars
 *                                  matching the given key:
 *                                  0: if no match
 *                                  The length of the key string: if a complete
 *                                                                match.
 *                                  Pass NULL if you don't care.
 * @param masked                True if the masked records preference is
 *                                  enabled.
 *
 * @return                      True if a record is found; false if no string is
 *                                  passed or there aren't any records.
 */
extern Boolean AddrDBLookupString (DmOpenRef dbP, Char *key,
                                   Boolean sortByCompany, UInt16 category,
                                   UInt16 *recordP, Boolean *completeMatch,
                                   UInt16* oNumMatchingCharsP, Boolean masked);

#ifdef __cplusplus
}
#endif

#endif // ADDRESSDB_H
