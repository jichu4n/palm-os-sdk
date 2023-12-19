/*************************************************************************
 *
 * Copyright (c) 2001-2002 by AlphaSmart, Inc.
 * All rights reserved.
 *
 * FILE:            WideTallApp.r
 *
 * DESCRIPTION: This file contains resources for WideTall applications.
 *
 ****************************************************************************/


type 'wTap' {
    //Currently no data is stored in resource, need dummy otherwise 
    //resource is not linked in
    unsigned longint;   
};


resource 'wTap' (1000, "WideTall Aware")
{
    0x00000001; //Set bit 0 for wide tall aware.
                
};
