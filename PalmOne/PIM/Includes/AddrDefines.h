/******************************************************************************
 * Copyright (c) 1995-2005 palmOne, Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/
/**
 * @ingroup PIM
 */

/**
 * @file AddrDefines.h
 *
 * @brief Contains Contacts database record field, field label, and auto-fill
 *        database constants and macros.
 *
 * <hr>
 */

#ifndef ADDRDEFINES_H
#define ADDRDEFINES_H

#include <Event.h>
#include <palmOne_68K.h>

/***********************************************************************
 *  Contacts database type and name.
 ***********************************************************************/

#define addrDBName          "ContactsDB-PAdd"   /**< Contacts DB Name */
#define addrDBType          'DATA'              /**< Contacts DB Type */
#define addrFileCAddress    'PAdd'              /**< Contacts DB Creator ID */

/***********************************************************************
 *  AutoFill database types and names.
 *  Note that a prefix of "Address" is used to avoid name conflicts
 *  with Expense app.
 ***********************************************************************/

/** AutoFill database for the title field. */
/*@{*/
#define titleDBType                         'titl'
#define titleDBName                         "AddressTitlesDB"
/*@}*/

/** AutoFill database for the company field. */
/*@{*/
#define companyDBType                       'cmpy'
#define companyDBName                       "AddressCompaniesDB"
/*@}*/

/** AutoFill database for the city field. */
/*@{*/
#define cityDBType                          'city'
#define cityDBName                          "AddressCitiesDB"
/*@}*/

/** AutoFill database for the state field. */
/*@{*/
#define stateDBType                         'stat'
#define stateDBName                         "AddressStatesDB"
/*@}*/

/** AutoFill database for the country field. */
/*@{*/
#define countryDBType                       'cnty'
#define countryDBName                       "AddressCountriesDB"
/*@}*/

/***********************************************************************
 *  Update codes.
 *  These codes are used to determine how the List View should be
 *  redrawn on a frmUpdateEvent.
 ***********************************************************************/
/**
 * @name List View Update Codes
 */
/*@{*/
#define updateRedrawAll                     0x01
#define updateGrabFocus                     0x02
#define updateItemHide                      0x04
#define updateCategoryChanged               0x08
#define updateFontChanged                   0x0100
#define updateListViewPhoneChanged          0x20
#define updateCustomFieldLabelChanged       0x40
#define updateSelectCurrentRecord           0x80
/*@}*/

/** Special update event sent when the custom field labels are renamed. */
#define kFrmCustomUpdateEvent               firstUserEvent

/***********************************************************************
 *  Common field string literals.
 ***********************************************************************/

/**
 * @name Common String Literals and Constants
 */
/*@{*/
#define shortenedFieldString                "..."
#define shortenedFieldLength                3
#define fieldSeparatorString                ", "
#define fieldSeparatorLength                2
#define spaceBetweenNamesAndPhoneNumbers    6
#define editFirstFieldIndex                 0
#define kDialListShowInListPhoneIndex       ((UInt16)(-1))
/*@}*/

/***********************************************************************
 *  Field and field label numbers.
 ***********************************************************************/

#define numPhoneLabels          8 /**< Number of phone labels */
#define numAddressLabels        3 /**< Number of address labels */
#define numChatLabels           5 /**< Number of chat labels */

/** Total number of fields related to birthday */
#define numBirthdayFields       3

/** Total number of fields in Contacts application */
#define addrNumFields           43

/** Number of fields that are non-string */
#define addrNumNonStringFields  4

/** Number of fields that will show up in UI */
#define addrNumDisplayFields    addrNumFields - (numBirthdayFields -1)

/** Number of fields that are of type string */
#define addrNumStringFields     addrNumFields - addrNumNonStringFields

/***********************************************************************
 *  Miscellaneous.
 ***********************************************************************/

/** Record index value indicating no record in the database. */
#define noRecord                    0xffff

/** Width of the field label popup trigger arrow. */
#define popupArrowSpacing           12

/** @brief Default phone field to show in List View.
 *
 *  Current default is the mobile field. This constant is not the
 *  mobileLabel constant.  It is actually the phone field which has
 *  mobile as the defualt label when a new record is created.
 */
#define defaultShowInList           2

/** Number of possible address fields (address fields meaning as blocks of
 *  street, city, etc.) e.g.: address, address2, address3
 */
#define deliveryAddressCount        3

/** Number of address sub-fields (sub-fields meaning address, city, state,
 *  zipCode, country.)
 */
#define deliveryAddressFieldCount   5

/***********************************************************************
 *
 *  Macros
 *
 ***********************************************************************/

/***********************************************************************
 *  Field label macros.
 ***********************************************************************/

/**
 * Get the phone field's label index.
 * The index returned should be an AddressPhoneLabels enum,
 * where it acts as an offset from the start of the phone label
 * strings.  However, not all the phone label strings are stored
 * consecutively.  Thus, if the index value is greater than
 * numPhoneLabelsStoredFirst, the label should be found in
 * the second set of phone label strings, where the returned
 * index value minus the numPhoneLabelsStoredFirst is equivalent
 * to the offset - 1 of the start of the second set of strings,
 * phoneLabelSecondStart.
 *
 * @param r Ptr to a record from which to retrieve the label.
 * @param p Index of the phone field to get.
 *
 * @return  The phone field label index corresponding to the given field.
 */
#define GetPhoneLabel(r, p) (((r)->options.phoneBits >> \
                            (((p) - firstPhoneField) << 2)) & 0xF)

/**
 * Set the phone field's label index.
 * The new phone label index should be an AddressPhoneLabels enum.
 *
 * @param r     Ptr to a record for which to set the label.
 * @param p     Index of the phone field to set.
 * @param pl    Index of the new phone label for the field.
 *
 * @return      Nothing.
 */
#define SetPhoneLabel(r, p, pl) ((r)->options.phoneBits =               \
                                ((r)->options.phoneBits & ~((UInt32)    \
                                0x0000000F << (((p) -                   \
                                firstPhoneField) << 2))) |              \
                                ((UInt32) pl << (((p) -                 \
                                firstPhoneField) << 2)));

/**
 * Get the address field's label index.
 * The index returned should be an AddressStreetAddressLabels enum.
 *
 * @param r Ptr to a record from which to retrieve the label.
 * @param p Index of the address field to get, meaning the
 *              1st, 2nd, or 3rd address set of fields, not
 *              individual fields within an address, like state.
 *
 * @return  The address field label index corresponding to the given field.
 */
#define GetAddressLabel(r, p) (((r)->options.addressBits >> ((((p)  \
                               - address)/5) << 2)) & 0xF)

/**
 * Set the address field's label index.
 * The new address label index should be an AddressStreetAddressLabels enum.
 *
 * @param r     Ptr to a record for which to set the label.
 * @param p     Index of the address field to set.
 * @param pl    Index of the new address label for the field.
 *
 * @return      Nothing.
 */
#define SetAddressLabel(r, p, pl) ((r)->options.addressBits =    \
                                  ((r)->options.addressBits &    \
                                  ~((UInt32) 0x0000000F <<       \
                                  ((((p) - address)/5) << 2))) | \
                                  ((UInt32) pl <<                \
                                  ((((p) - address)/5) << 2)));

/**
 * Get the chat field's label index.
 * The index returned should be an AddressChatLabels enum.
 * Note that this index acts as an offset from the start
 * of the chat label strings.
 *
 * @param r Ptr to a record from which to retrieve the label.
 * @param p Index of the chat field to get.
 *
 * @return  The chat field label index corresponding to the given field.
 */
#define GetChatLabel(r, p) (((r)->options.chatBits >> (((p) - chat1) << 2)) & \
                            0xF)

/**
 * Set the chat field's label index.
 * The new chat label index should be an AddressChatLabels enum.
 *
 * @param r     Ptr to a record for which to set the label.
 * @param p     Index of the chat field to set.
 * @param pl    Index of the new chat label for the field.
 *
 * @return      Nothing.
 */
#define SetChatLabel(r, p, pl) ((r)->options.chatBits =                 \
                               ((r)->options.chatBits & ~((UInt32)      \
                               0x0000000F << (((p) - chat1) << 2))) |   \
                               ((UInt32) pl << (((p) - chat1) << 2)));

/**
 * Get the phone field's label string.
 * The given label number should be an AddressPhoneLabels enum.
 * e.g.: to retrieve the phone label for a given record's phone
 * field, use GetPhoneLabel() to get the label number to pass
 * to this macro.
 *
 * @param appInfoP  Ptr to the application info block containing
 *                      the field labels.
 * @param labelNum  Index of the phone field label to get.
 *
 * @return          A phone field label.
 */
#define GetPhoneLabelText(appInfoP, labelNum) \
        ((appInfoP)->fieldLabels[(((labelNum) < numPhoneLabelsStoredFirst) ? \
        (labelNum) + firstPhoneField : phoneLabelSecondStart)])

/**
 * Get the chat field's label string.
 * The given label number should be an AddressChatLabels enum.
 * e.g.: to retrieve the chat label for a given record's chat
 * field, use GetChatLabel() to get the label number to pass
 * to this macro.
 *
 * @param appInfoP  Ptr to the application info block containing
 *                      the field labels.
 * @param labelNum  Index of the chat field label to get.
 *
 * @return          A chat field label.
 */
#define GetChatLabelText(appInfoP, labelNum) \
        ((appInfoP)->fieldLabels[(((labelNum) < numChatLabels) ? \
        (labelNum) + chatLabelstart : chatLabelstart)])

/**
 * Get the address field's label string.
 * The given label number should be an AddressStreetAddressLabels enum.
 * e.g.: to retrieve the address label for a given record's address
 * field, use GetAddressLabel() to get the label number to pass
 * to this macro.
 *
 * @param appInfoP  Ptr to the application info block containing
 *                      the field labels.
 * @param labelNum  Index of the address field label to get.
 *
 * @return          An address field label.
 */
#define GetAddrLabelText(appInfoP, labelNum) \
        ((appInfoP)->fieldLabels[address + (labelNum * 5)])

/***********************************************************************
 *  Address field parts (or sub-field) macros.
 *  Note: For addresses, the address, city, state, zipCode, country, are
 *        accessed relative to a base address field index.
 ***********************************************************************/

/**
 * Get the index of the street sub-field of the address field.
 * The address base field index corresponds to one of the
 * address field indices in the AddressFields enum.  e.g.:
 * address2.
 *
 * @param b Address base field index.
 *
 * @return  The street field index.
 */
#define DeliveryAddressStreetField(b)           (b)

/**
 * Get the index of the city sub-field of the address field.
 * The address base field index corresponds to one of the
 * address field indices in the AddressFields enum.  e.g.:
 * address2.
 *
 * @param b Address base field index.
 *
 * @return  The city field index.
 */
#define DeliveryAddressCityField(b)             ((b) + 1)

/**
 * Get the index of the state sub-field of the address field.
 * The address base field index corresponds to one of the
 * address field indices in the AddressFields enum.  e.g.:
 * address2.
 *
 * @param b Address base field index.
 *
 * @return  The state field index.
 */
#define DeliveryAddressStateField(b)            ((b) + 2)

/**
 * Get the index of the zip code sub-field of the address field.
 * The address base field index corresponds to one of the
 * address field indices in the AddressFields enum.  e.g.:
 * address2.
 *
 * @param b Address base field index.
 *
 * @return  The zip code field index.
 */
#define DeliveryAddressZipCodeField(b)          ((b) + 3)

/**
 * Get the index of the country sub-field of the address field.
 * The address base field index corresponds to one of the
 * address field indices in the AddressFields enum.  e.g.:
 * address2.
 *
 * @param b Address base field index.
 *
 * @return  The country field index.
 */
#define DeliveryAddressCountryField(b)          ((b) + 4)

/***********************************************************************
 *  Field predicate macros.
 ***********************************************************************/

/**
 * Is the field a custom type field.
 * Note that "custom" here is in regard to the type of table
 * field, not to the custom/renameable record fields.
 *
 * @param fieldIndex    Index of the field to check.
 *
 * @return              True if a custom field.
 */
#define isCustomField(fieldIndex) (fieldIndex == birthdayDate ||    \
                                   fieldIndex == pictureInfo )

/**
 * Is the field a selector trigger type.
 *
 * @param fieldIndex    Index of the field to check.
 *
 * @return              True if a selector trigger type field.
 */
#define isSelectorTypeField(fieldIndex) (fieldIndex == birthdayDate)

/** Identifies picture fields */
#define isPictureField(fieldIndex) (fieldIndex == pictureInfo)

/**
 * Is the field a phone field.
 *
 * @param fieldIndex    Index of the field to check.
 *
 * @return              True if a phone field.
 */
#define isPhoneField(f) (f >= firstPhoneField && f <= lastPhoneField)

/**
 * Is the field an address field.
 *
 * @param fieldIndex    Index of the field to check.
 *
 * @return              True if an address field.
 */
#define isAddressField(fieldIndex) (fieldIndex == address || fieldIndex == address2 || fieldIndex == address3)

/**
 * Is the field a chat field.
 *
 * @param fieldIndex    Index of the field to check.
 *
 * @return              True if a chat field.
 */
#define isChatField(fieldIndex) (fieldIndex == chat1 || fieldIndex ==chat2)

/**
 * Does the field have a popup list of labels in Edit View.
 *
 * @param fieldIndex    Index of the field to check.
 *
 * @return              True if the field has a popup list of labels.
 */
#define isPopupTypeField(fieldIndex) (isPhoneField(fieldIndex)   || \
                                      isAddressField(fieldIndex) || \
                                      isChatField(fieldIndex))

/**
 * Is the label a chat field label.
 *
 * @param fieldIndex    Index of the field label to check.
 *
 * @return              True if a chat field label.
 */
#define isChatLabel(fieldIndex) (fieldIndex >= chatLabelstart && fieldIndex < \
                                 chatLabelstart + numChatLabels)

/***********************************************************************
 *  Miscellaneous macros.
 ***********************************************************************/

/**
 * Get the number of chat fields.
 *
 * @return The number of chat fields.
 */
#define chatFieldCount ((lastChatField - firstChatField) + 1)

/**
 * Get the number of custom/renameable fields.
 *
 * @return The number of custom/renameable fields.
 */
#define customFieldCount ((lastRenameableLabel - firstRenameableLabel) + 1)

/** Picture height and width in low res pixels */
/*@{*/
#define pictureHeight               48
#define pictureWidth                48
/*@}*/

/** Max picture size stroed in blob (in low res) */
/*@{*/
#define pictureMaxHeight            72
#define pictureMaxWidth             72
/*@}*/

#define ftrCreateDefaultDatabase    0
#define addrPrefVersionNum          0x01 //0x04
#define addrPrefID                  0x00

#endif  // ADDRDEFINES_H
