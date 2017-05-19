#include <sys/types.h>
#include <functional>
#include <string.h>
#include <stdio.h>
using namespace std;
#include <iomanip>
#include "error.h"
#include "utility.h"
#include "catalog.h"

// define if debug output wanted


//
// Retrieves and prints information from the catalogs about the for
// the user. If no relation is given (relation.empty() is true), then
// it lists all the relations in the database, along with the width in
// bytes of the relation, the number of attributes in the relation,
// and the number of attributes that are indexed.  If a relation is
// given, then it lists all of the attributes of the relation, as well
// as its type, length, and offset, whether it's indexed or not, and
// its index number.
//
// Returns:
// 	OK on success
// 	error code otherwise
//

const Status RelCatalog::help(const string & relation)
{
  Status status;
  RelDesc rd;
  AttrDesc *attrs;
  int attrCnt;

  if (relation.empty()) return UT_Print(RELCATNAME);

status=relCat->getInfo(relation,rd);

status=attrCat->getRelInfo(relation,attrCnt,attrs);

cout << "Rel Name : " << rd.relName << endl;

cout << "A-Name        A-type    A-length  A-offset" << endl;
cout << "------------- --------- --------- ---------" << endl;


for(int i=0; i <attrCnt; i++){
if (attrs[i].attrType==0){
cout << left << setw(15) << attrs[i].attrName << setw(10) << "string";
cout << setw(13) << attrs[i].attrLen << setw(10) << attrs[i].attrOffset;
cout << endl;
}
else if (attrs[i].attrType==1){
cout << left << setw(15) << attrs[i].attrName << setw(10) << "integer";
cout << setw(13) << attrs[i].attrLen << setw(10) << attrs[i].attrOffset;
cout << endl;
}
else{ 
cout << left << setw(15) << attrs[i].attrName << setw(10) << "float";
cout << setw(13) << attrs[i].attrLen << setw(10) << attrs[i].attrOffset;
cout << endl;
}

}
  return OK;
}
