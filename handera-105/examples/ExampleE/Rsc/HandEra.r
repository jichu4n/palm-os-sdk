/***********************************************************************
 *
 * Copyright (c) 1999-2001, TRG, All Rights Reserved
 *
 * PROJECT:         HandEra 330
 *
 * FILE:            HandEra.r
 *
 * DESCRIPTION:     
 *
 * AUTHOR:          John Ehm
 *
 * DATE:            01/17/01
 *
 **********************************************************************/


type 'sKst' {
    //Currently no data is stored in resource, need dummy otherwise 
    //resource is not linked in
    unsigned longint;   
};


resource 'sKst' (1000, "HandEra Aware")
{
    0x00000000;

};
