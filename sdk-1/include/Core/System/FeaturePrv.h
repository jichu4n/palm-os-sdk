/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1994, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		FeaturePrv.h
 *
 * Description:
 *		Private Header for the Feature Manager
 *
 * History:
 *   	8/9/95  RM - Created by Ron Marianetti
 *
 *******************************************************************/

#ifdef NON_PORTABLE
#ifndef __FEATUREPRV_H__
#define __FEATUREPRV_H__




/************************************************************
 * Structure of a Feature Table
 *************************************************************/
typedef struct FtrFeatureType {
	UInt		num;										// feature number
	DWord		value;									// feature value
	} FtrFeatureType;
typedef FtrFeatureType* FtrFeaturePtr;

typedef struct FtrCreatorType {
	DWord				creator;							// feature creator
	UInt				numEntries;						// # of entries
	FtrFeatureType	feature[1];						// variable size array of Features
	} FtrCreatorType;
typedef FtrCreatorType*	FtrCreatorPtr;

typedef struct FtrTableType {
	UInt				numEntries;						// # of entries
	FtrCreatorType	creator[1];						// var. size array of Creators
	} FtrTableType;
typedef FtrTableType*	FtrTablePtr;
typedef FtrTablePtr*		FtrTableHand;


/************************************************************
 * Structure of a Feature Manager Globals
 *************************************************************/
typedef struct FtrGlobalsType {
	FtrTableHand		romTableH;					// ROM table of features
	FtrTableHand		ramTableH;					// RAM table of features
	} FtrGlobalsType;
typedef	FtrGlobalsType*	FtrGlobalsPtr;



/************************************************************
 *	Private structures
 *************************************************************/
// This structure created for temporary use within calls
typedef struct FtrCacheType {
	FtrTableHand		romTableH;					// ROM table of features
	FtrTableHand		ramTableH;					// RAM table of features
	FtrTablePtr			romTableP;
	FtrTablePtr			ramTableP;
	} FtrCacheType;
typedef FtrCacheType*	FtrCachePtr;





#endif // __FEATUREPRV_H__
#endif // NON_PORTABLE
