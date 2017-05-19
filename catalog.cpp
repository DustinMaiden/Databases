#include "catalog.h"


RelCatalog::RelCatalog(Status &status) :
	 HeapFile(RELCATNAME, status)
{
// nothing should be needed here
}


const Status RelCatalog::getInfo(const string & relation, RelDesc &record)
{
  if (relation.empty())
    return BADCATPARM;

  Status status;
  Record rec;
  RID rid;

//object on relcat relation
HeapFileScan* hscanner= new HeapFileScan(RELCATNAME,status);

//begin scan on relation using startScan
status=hscanner->startScan(0,0,STRING,NULL,EQ);//review parameters

//while scanning succeeds
while(status==OK){

//use scannext and getrecord to get desired tuple
status=hscanner->scanNext(rid);
if(status==OK){
status=hscanner->getRecord(rec);
if(status!=OK) return RELNOTFOUND;//break out

//compare relation name with record
if(relation.compare(((RelDesc*)rec.data)->relName)==0){
memcpy(&record,rec.data,rec.length); // pull out of buffer pool

return status;
}
}
}
delete hscanner;//cleanup object
return RELNOTFOUND;

}


const Status RelCatalog::addInfo(RelDesc & record)
{
  RID rid;
  InsertFileScan*  ifs;
  Status status;
  Record record1;

//setting up record
record1.data=&record;
record1.length=sizeof(RelDesc);

//create IFS objct on relation catalog table
ifs=new InsertFileScan(RELCATNAME,status);

//create record and insert into relation catalog table
status = ifs->insertRecord(record1,rid);

delete ifs;//cleanup object
return status;
}

const Status RelCatalog::removeInfo(const string & relation)
{
  Status status;
  RID rid;
  HeapFileScan*  hscanner2;
  Record record2;
  if (relation.empty()) return BADCATPARM;

//object on relcat relation
hscanner2=new HeapFileScan(RELCATNAME,status);

//begin scan as done previously in order to find tuple for removal
status=hscanner2->startScan(0,0,STRING,NULL,EQ);

//continue as long as OK
while(status==OK){
status=hscanner2->scanNext(rid);//scannext for finding tuple
if(status!=OK) return RELNOTFOUND;
else{
status=hscanner2->getRecord(record2);//getrecord for desired tule

//compare relation name with record
if(relation.compare(((RelDesc*)record2.data)->relName)==0){
hscanner2->deleteRecord();//delete tuple

return status;
}
}
}
delete hscanner2;//cleanup object

return RELNOTFOUND;

}


RelCatalog::~RelCatalog()
{
// nothing should be needed here
}


AttrCatalog::AttrCatalog(Status &status) :
	 HeapFile(ATTRCATNAME, status)
{
// nothing should be needed here
}


const Status AttrCatalog::getInfo(const string & relation, 
				  const string & attrName,
				  AttrDesc &record)
{

  Status status;
  RID rid;
  Record record3;
  HeapFileScan*  hscanner3;
  AttrDesc* xDesc;

  if (relation.empty() || attrName.empty()) return BADCATPARM;

//object for scanning 
hscanner3=new HeapFileScan(ATTRCATNAME,status);
if(status==OK)

status=hscanner3->startScan(0,0,STRING,NULL,EQ);
if(status==OK)

//check each tuple to find desired tuple
status=hscanner3->scanNext(rid);

//while scanning is finding recods
while(status ==OK){

//get desired tuple
status=hscanner3->getRecord(record3);
if(status==OK){

//attrcat tuples are of type AttrDesc
xDesc = (AttrDesc*)record3.data;

//comparison
if(attrName.compare(xDesc->attrName)==0 && relation.compare(xDesc->relName)==0){
record = *xDesc;
}
}
}

delete hscanner3;//object cleanup
delete xDesc;//object cleanup
return status;
}


const  Status AttrCatalog::addInfo(AttrDesc & record)
{
  RID rid;
  InsertFileScan*  ifs;
  Status status;
  Record record4;
//adds a tupleto attrcat relation
//IFS object
ifs = new InsertFileScan(ATTRCATNAME,status);
if(status!=OK) return status;//EH

//setup record
record4.data=&record;
record4.length=sizeof(AttrDesc);
//insert tuple
status=ifs->insertRecord(record4,rid);

delete ifs;//object cleanup
return status;
}


const Status AttrCatalog::removeInfo(const string & relation, 
			       const string & attrName)
{
  Status status;
  Record rec;
  RID rid;
  AttrDesc record;
  HeapFileScan*  hscanner5;

  if (relation.empty() || attrName.empty()) return BADCATPARM;

//object creation for scanning
hscanner5 = new HeapFileScan(ATTRCATNAME,status);
if(status==OK){
status=hscanner5->startScan(0,0,STRING,NULL,EQ);
if(status==OK){
while(status==OK){
status=hscanner5->scanNext(rid);
if(status==OK)
status=hscanner5->HeapFile::getRecord(rid,rec);
if(status==OK){

//compare names
if ( relation.compare(((AttrDesc*)rec.data)->relName)==0 && attrName.compare(((AttrDesc*)rec.data)->attrName)==0 ) { 

//remove this record 
status = hscanner5->deleteRecord(); 
}
}
}
}
}

delete hscanner5;//object cleanup
return status;
}


const Status AttrCatalog::getRelInfo(const string & relation, 
				     int &attrCnt,
				     AttrDesc *&attrs)
{
  Status status;
  RID rid;
  Record record4;
  HeapFileScan*  hscanner4;
  RelDesc relDesc;
  AttrDesc* yDesc;
  int x=0;
  if (relation.empty()) return BADCATPARM;

//returns by ref. descriptors for all attriutes
status=relCat->getInfo(relation,relDesc);
if(status!=OK) return status;

//setup attribute count
attrCnt=relDesc.attrCnt;

//object creation for scan
hscanner4 = new HeapFileScan(ATTRCATNAME,status);
if(status==OK){
status=hscanner4->startScan(0,0,STRING,NULL,EQ);
if(status==OK){
//attrs array with slots for each attribute
attrs = new AttrDesc[attrCnt];

//loop through atributes
while(x<attrCnt){
status=hscanner4->scanNext(rid);
if(status!=OK) return status;

//get desired tuple
status=hscanner4->HeapFile::getRecord(rid,record4);
if(status==OK){
yDesc = (AttrDesc*)record4.data;

if(relation.compare(yDesc->relName)==0){
attrs[x] = *yDesc;
x++;
}
}
}
}
}
delete hscanner4;//object cleanup
return status;
}


AttrCatalog::~AttrCatalog()
{
// nothing should be needed here
}

