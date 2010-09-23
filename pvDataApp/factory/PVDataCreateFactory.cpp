/*PVDataCreateFactory.cpp*/
#include <cstddef>
#include <cstdlib>
#include <string>
#include <cstdio>
#include "pvData.h"
#include "factory.h"
#include "AbstractPVField.h"
#include "AbstractPVScalar.h"
#include "AbstractPVArray.h"
#include "AbstractPVScalarArray.h"
#include "BasePVDouble.h"
#include "AbstractPVArray.h"
#include "BasePVDoubleArray.h"
#include "BasePVStructure.h"
#include "BasePVStructureArray.h"

namespace epics { namespace pvData {

   static std::string notImplemented("not implemented");

   static FieldCreate * fieldCreate = 0;
   static PVDataCreate* pvDataCreate = 0;

   PVDataCreate::PVDataCreate(){};

   PVField *PVDataCreate::createPVField(PVStructure *parent,
           FieldConstPtr field)
   {
        switch(field->getType()) {
        case scalar:
            return createPVScalar(parent,(ScalarConstPtr)field);
        case scalarArray:
            return (PVField *)createPVScalarArray(parent,
                (ScalarArrayConstPtr)field);
        case structure:
            return (PVField *)createPVStructure(parent,
                (StructureConstPtr)field);
        case structureArray:
            return createPVStructureArray(parent,
                (StructureArrayConstPtr)field);
        }
        std::string message("PVDataCreate::createPVField");
        throw std::invalid_argument(message);
   };

   PVField *PVDataCreate::createPVField(PVStructure *parent,
           StringConstPtr fieldName,PVField * fieldToClone)
   {
        switch(fieldToClone->getField()->getType()) {
        case scalar:
            return createPVScalar(parent,fieldName,(PVScalar*)fieldToClone);
        case scalarArray:
            return (PVField *)createPVScalarArray(parent,fieldName,
                (PVScalarArray *)fieldToClone);
        case structure:
            return (PVField *)createPVStructure(parent,fieldName,
                (PVStructure *)fieldToClone);
        case structureArray:
           std::string message(
           "PVDataCreate::createPVField structureArray not valid fieldToClone");
           throw std::invalid_argument(message);
        }
        std::string message("PVDataCreate::createPVField");
        throw std::logic_error(message);
   };

   PVScalar *PVDataCreate::createPVScalar(PVStructure *parent,ScalarConstPtr scalar)
   {
        ScalarType scalarType = scalar->getScalarType();
        switch(scalarType) {
        case pvDouble:
            return new BasePVDouble(parent,scalar);
        default:
            throw std::logic_error(notImplemented);
        }
        throw std::logic_error(notImplemented);
   };

   PVScalar *PVDataCreate::createPVScalar(PVStructure *parent,
           StringConstPtr fieldName,ScalarType scalarType)
   {
        if(fieldCreate==0) fieldCreate = getFieldCreate();
        ScalarConstPtr scalar = fieldCreate->createScalar(fieldName,scalarType);
        return createPVScalar(parent,scalar);
   };

   PVScalar *PVDataCreate::createPVScalar(PVStructure *parent,
           StringConstPtr fieldName,PVScalar * scalarToClone)
   {
        PVScalar *pvScalar = createPVScalar(parent,fieldName,
            scalarToClone->getScalar()->getScalarType());
        //MARTY MUST CALL CONVERT
        //MARTY MUST COPY AUXInfo
        return pvScalar;
   };

   PVScalarArray *PVDataCreate::createPVScalarArray(PVStructure *parent,
           ScalarArrayConstPtr scalarArray)
   {
        switch(scalarArray->getElementType()) {
        case pvBoolean: break;
        case pvByte:    break;
        case pvShort:   break;
        case pvInt:     break;
        case pvLong:    break;
        case pvFloat:   break;
        case pvDouble:
              return new BasePVDoubleArray(parent,scalarArray);
        case pvString:  break;
        }
        throw std::logic_error(notImplemented);
        
   };

   PVScalarArray *PVDataCreate::createPVScalarArray(PVStructure *parent,
           StringConstPtr fieldName,ScalarType elementType)
   {
        throw std::logic_error(notImplemented);
   };

   PVScalarArray *PVDataCreate::createPVScalarArray(PVStructure *parent,
           StringConstPtr fieldName,PVScalarArray * scalarArrayToClone)
   {
        throw std::logic_error(notImplemented);
   };

   PVStructureArray *PVDataCreate::createPVStructureArray(PVStructure *parent,
           StructureArrayConstPtr structureArray)
   {
        throw std::logic_error(notImplemented);
   };

   PVStructure *PVDataCreate::createPVStructure(PVStructure *parent,
           StructureConstPtr structure)
   {
        return new BasePVStructure(parent,structure);
   };

   PVStructure *PVDataCreate::createPVStructure(PVStructure *parent,
           StringConstPtr fieldName,FieldConstPtrArray fields)
   {
        throw std::logic_error(notImplemented);
   };

   PVStructure *PVDataCreate::createPVStructure(PVStructure *parent,
           StringConstPtr fieldName,PVStructure *structToClone)
   {
        throw std::logic_error(notImplemented);
   };


   class PVDataCreateExt : public PVDataCreate {
   public:
       PVDataCreateExt(): PVDataCreate(){};
   };

    PVDataCreate * getPVDataCreate() {
           if(pvDataCreate==0) pvDataCreate = new PVDataCreateExt();
            return pvDataCreate;
    }

}}