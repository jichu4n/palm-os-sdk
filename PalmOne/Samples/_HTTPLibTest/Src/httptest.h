/* httptest.h */

#ifndef __HTTPTEST_H__
#define __HTTPTEST_H__

Err PrvHTTPLibInitialize(void);
Err PrvHTTPLibFinalize(void);
void PrvHTTPLibTestGet(Char *urlP);
void PrvHTTPLibTestPost(Char *urlP, Char *postdataP);


#endif