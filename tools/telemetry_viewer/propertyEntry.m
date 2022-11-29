classdef propertyEntry
    properties
        name
        type
        castFunc
        id
        numBytes
        rw
        dispType
        dispDims
        eltNames
        numElts
        processData
        processDataFunc
        valToString
    end

    methods
        function p = propertyEntry(name, type, castFunc, id, numBytes, rw, dispType, processData, func, dispDims, eltNames, valToString)
            p.name = name;
            p.type = type;
            p.castFunc = castFunc;
            p.id = id;
            p.numBytes = numBytes;
            p.rw = rw;
            p.dispType = dispType;
            p.dispDims = dispDims;
            p.eltNames = eltNames;
            p.processData  = processData;
            p.processDataFunc = func;
            p.numElts = sum(p.dispDims);
            if nargin == 12
                p.valToString = valToString;
            end
        end
    end
end