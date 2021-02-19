//
// Generated file, do not edit! Created by nedtool 5.6 from Update.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "Update_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(UpdateReq)

UpdateReq::UpdateReq(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->source = 0;
    this->destination = 0;
    report1_arraysize = 0;
    this->report1 = 0;
    report2_arraysize = 0;
    this->report2 = 0;
    this->treeID = 0;
    this->mac = 0;
    proof_arraysize = 0;
    this->proof = 0;
}

UpdateReq::UpdateReq(const UpdateReq& other) : ::omnetpp::cPacket(other)
{
    report1_arraysize = 0;
    this->report1 = 0;
    report2_arraysize = 0;
    this->report2 = 0;
    proof_arraysize = 0;
    this->proof = 0;
    copy(other);
}

UpdateReq::~UpdateReq()
{
    delete [] this->report1;
    delete [] this->report2;
    delete [] this->proof;
}

UpdateReq& UpdateReq::operator=(const UpdateReq& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void UpdateReq::copy(const UpdateReq& other)
{
    this->source = other.source;
    this->destination = other.destination;
    delete [] this->report1;
    this->report1 = (other.report1_arraysize==0) ? nullptr : new int[other.report1_arraysize];
    report1_arraysize = other.report1_arraysize;
    for (unsigned int i=0; i<report1_arraysize; i++)
        this->report1[i] = other.report1[i];
    delete [] this->report2;
    this->report2 = (other.report2_arraysize==0) ? nullptr : new int[other.report2_arraysize];
    report2_arraysize = other.report2_arraysize;
    for (unsigned int i=0; i<report2_arraysize; i++)
        this->report2[i] = other.report2[i];
    this->treeID = other.treeID;
    this->mac = other.mac;
    delete [] this->proof;
    this->proof = (other.proof_arraysize==0) ? nullptr : new int[other.proof_arraysize];
    proof_arraysize = other.proof_arraysize;
    for (unsigned int i=0; i<proof_arraysize; i++)
        this->proof[i] = other.proof[i];
}

void UpdateReq::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->source);
    doParsimPacking(b,this->destination);
    b->pack(report1_arraysize);
    doParsimArrayPacking(b,this->report1,report1_arraysize);
    b->pack(report2_arraysize);
    doParsimArrayPacking(b,this->report2,report2_arraysize);
    doParsimPacking(b,this->treeID);
    doParsimPacking(b,this->mac);
    b->pack(proof_arraysize);
    doParsimArrayPacking(b,this->proof,proof_arraysize);
}

void UpdateReq::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->source);
    doParsimUnpacking(b,this->destination);
    delete [] this->report1;
    b->unpack(report1_arraysize);
    if (report1_arraysize==0) {
        this->report1 = 0;
    } else {
        this->report1 = new int[report1_arraysize];
        doParsimArrayUnpacking(b,this->report1,report1_arraysize);
    }
    delete [] this->report2;
    b->unpack(report2_arraysize);
    if (report2_arraysize==0) {
        this->report2 = 0;
    } else {
        this->report2 = new int[report2_arraysize];
        doParsimArrayUnpacking(b,this->report2,report2_arraysize);
    }
    doParsimUnpacking(b,this->treeID);
    doParsimUnpacking(b,this->mac);
    delete [] this->proof;
    b->unpack(proof_arraysize);
    if (proof_arraysize==0) {
        this->proof = 0;
    } else {
        this->proof = new int[proof_arraysize];
        doParsimArrayUnpacking(b,this->proof,proof_arraysize);
    }
}

int UpdateReq::getSource() const
{
    return this->source;
}

void UpdateReq::setSource(int source)
{
    this->source = source;
}

int UpdateReq::getDestination() const
{
    return this->destination;
}

void UpdateReq::setDestination(int destination)
{
    this->destination = destination;
}

void UpdateReq::setReport1ArraySize(unsigned int size)
{
    int *report12 = (size==0) ? nullptr : new int[size];
    unsigned int sz = report1_arraysize < size ? report1_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        report12[i] = this->report1[i];
    for (unsigned int i=sz; i<size; i++)
        report12[i] = 0;
    report1_arraysize = size;
    delete [] this->report1;
    this->report1 = report12;
}

unsigned int UpdateReq::getReport1ArraySize() const
{
    return report1_arraysize;
}

int UpdateReq::getReport1(unsigned int k) const
{
    if (k>=report1_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", report1_arraysize, k);
    return this->report1[k];
}

void UpdateReq::setReport1(unsigned int k, int report1)
{
    if (k>=report1_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", report1_arraysize, k);
    this->report1[k] = report1;
}

void UpdateReq::setReport2ArraySize(unsigned int size)
{
    int *report22 = (size==0) ? nullptr : new int[size];
    unsigned int sz = report2_arraysize < size ? report2_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        report22[i] = this->report2[i];
    for (unsigned int i=sz; i<size; i++)
        report22[i] = 0;
    report2_arraysize = size;
    delete [] this->report2;
    this->report2 = report22;
}

unsigned int UpdateReq::getReport2ArraySize() const
{
    return report2_arraysize;
}

int UpdateReq::getReport2(unsigned int k) const
{
    if (k>=report2_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", report2_arraysize, k);
    return this->report2[k];
}

void UpdateReq::setReport2(unsigned int k, int report2)
{
    if (k>=report2_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", report2_arraysize, k);
    this->report2[k] = report2;
}

int UpdateReq::getTreeID() const
{
    return this->treeID;
}

void UpdateReq::setTreeID(int treeID)
{
    this->treeID = treeID;
}

int UpdateReq::getMac() const
{
    return this->mac;
}

void UpdateReq::setMac(int mac)
{
    this->mac = mac;
}

void UpdateReq::setProofArraySize(unsigned int size)
{
    int *proof2 = (size==0) ? nullptr : new int[size];
    unsigned int sz = proof_arraysize < size ? proof_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        proof2[i] = this->proof[i];
    for (unsigned int i=sz; i<size; i++)
        proof2[i] = 0;
    proof_arraysize = size;
    delete [] this->proof;
    this->proof = proof2;
}

unsigned int UpdateReq::getProofArraySize() const
{
    return proof_arraysize;
}

int UpdateReq::getProof(unsigned int k) const
{
    if (k>=proof_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", proof_arraysize, k);
    return this->proof[k];
}

void UpdateReq::setProof(unsigned int k, int proof)
{
    if (k>=proof_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", proof_arraysize, k);
    this->proof[k] = proof;
}

class UpdateReqDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    UpdateReqDescriptor();
    virtual ~UpdateReqDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(UpdateReqDescriptor)

UpdateReqDescriptor::UpdateReqDescriptor() : omnetpp::cClassDescriptor("UpdateReq", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

UpdateReqDescriptor::~UpdateReqDescriptor()
{
    delete[] propertynames;
}

bool UpdateReqDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<UpdateReq *>(obj)!=nullptr;
}

const char **UpdateReqDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *UpdateReqDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int UpdateReqDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 7+basedesc->getFieldCount() : 7;
}

unsigned int UpdateReqDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<7) ? fieldTypeFlags[field] : 0;
}

const char *UpdateReqDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "source",
        "destination",
        "report1",
        "report2",
        "treeID",
        "mac",
        "proof",
    };
    return (field>=0 && field<7) ? fieldNames[field] : nullptr;
}

int UpdateReqDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "source")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destination")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "report1")==0) return base+2;
    if (fieldName[0]=='r' && strcmp(fieldName, "report2")==0) return base+3;
    if (fieldName[0]=='t' && strcmp(fieldName, "treeID")==0) return base+4;
    if (fieldName[0]=='m' && strcmp(fieldName, "mac")==0) return base+5;
    if (fieldName[0]=='p' && strcmp(fieldName, "proof")==0) return base+6;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *UpdateReqDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<7) ? fieldTypeStrings[field] : nullptr;
}

const char **UpdateReqDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *UpdateReqDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int UpdateReqDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    UpdateReq *pp = (UpdateReq *)object; (void)pp;
    switch (field) {
        case 2: return pp->getReport1ArraySize();
        case 3: return pp->getReport2ArraySize();
        case 6: return pp->getProofArraySize();
        default: return 0;
    }
}

const char *UpdateReqDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    UpdateReq *pp = (UpdateReq *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string UpdateReqDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    UpdateReq *pp = (UpdateReq *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSource());
        case 1: return long2string(pp->getDestination());
        case 2: return long2string(pp->getReport1(i));
        case 3: return long2string(pp->getReport2(i));
        case 4: return long2string(pp->getTreeID());
        case 5: return long2string(pp->getMac());
        case 6: return long2string(pp->getProof(i));
        default: return "";
    }
}

bool UpdateReqDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    UpdateReq *pp = (UpdateReq *)object; (void)pp;
    switch (field) {
        case 0: pp->setSource(string2long(value)); return true;
        case 1: pp->setDestination(string2long(value)); return true;
        case 2: pp->setReport1(i,string2long(value)); return true;
        case 3: pp->setReport2(i,string2long(value)); return true;
        case 4: pp->setTreeID(string2long(value)); return true;
        case 5: pp->setMac(string2long(value)); return true;
        case 6: pp->setProof(i,string2long(value)); return true;
        default: return false;
    }
}

const char *UpdateReqDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *UpdateReqDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    UpdateReq *pp = (UpdateReq *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


