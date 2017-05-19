#include "catalog.h"

//
// Destroys a relation. It performs the following steps:
//
// 	removes the catalog entry for the relation
// 	destroys the heap file containing the tuples in the relation
//
// Returns:
// 	OK on success
// 	error code otherwise
//

const Status RelCatalog::destroyRel(const string & relation)
{
  Status status;

  if (relation.empty() || 
      relation == string(RELCATNAME) || 
      relation == string(ATTRCATNAME))
    return BADCATPARM;

//remove all relevant info from relcat & attrcat
status=attrCat->dropRelation(relation);
status=relCat->removeInfo(relation);

//destry heapfile corresponding to relation
status=destroyHeapFile(relation);

return status;
}


//
// Drops a relation. It performs the following steps:
//
// 	removes the catalog entries for the relation
//
// Returns:
// 	OK on success
// 	error code otherwise
//

const Status AttrCatalog::dropRelation(const string & relation)
{
  Status status;
  AttrDesc *attrs;
  int attrCnt, i;

  if (relation.empty()) return BADCATPARM;

//get all attributes
status = getRelInfo(relation,attrCnt,attrs);
if(status!=OK) return status;

//remove attributes
for(int i=0;i<attrCnt;i++){
status=removeInfo(relation,attrs[i].attrName);
}

return status;
}


