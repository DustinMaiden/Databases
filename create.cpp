#include "catalog.h"

const Status RelCatalog::createRel(const string & relation, 
				   const int attrCnt,
				   const attrInfo attrList[])
{
  Status status;
  RelDesc rd;
  AttrDesc ad;

  if (relation.empty() || attrCnt < 1)
    return BADCATPARM;

  if (relation.length() >= sizeof rd.relName)
    return NAMETOOLONG;

//check if name already exists
status=getInfo(relation,rd);
if(status==OK) return RELEXISTS;// if relation already exists

//RELNOTFOUND
//Create relation
strcpy(rd.relName,relation.c_str());
rd.attrCnt=attrCnt; //take attibute count into relation
status = addInfo(rd); //add tuple to rel relation
if(status!=OK) return status; 

// for each attribute in the relation->add tuples into attr
for(int i=0; i<attrCnt; i++){
//copy values
strcpy(ad.relName,attrList[i].relName);
strcpy(ad.attrName,attrList[i].attrName);

ad.attrType = attrList[i].attrType;//type
ad.attrLen = attrList[i].attrLen;  //length
ad.attrOffset = i;                 //index

status=attrCat->addInfo(ad); 	   //add it

}
//create heap file instance to hold tuples
status = createHeapFile(relation);
return status;
}

