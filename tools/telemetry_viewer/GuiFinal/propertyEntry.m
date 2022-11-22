classdef propertyEntry
    properties
        name
        type
        id
        numBytes
        rw
        dispType
        dispDims
        eltNames
        numElts
    end

    methods
        function p = propertyEntry(name, type, id, numBytes, rw, dispType, dispDims, eltNames)
            p.name = name;
            p.type = type;
            p.id = id;
            p.numBytes = numBytes;
            p.rw = rw;
            p.dispType = dispType;
            p.dispDims = dispDims;
            p.eltNames = eltNames;
            p.numElts = sum(p.dispDims);
        end
    end
end