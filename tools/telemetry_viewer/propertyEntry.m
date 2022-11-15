classdef propertyEntry
  properties
    name
    type
    id
    numBytes
    rw
    dispType
    eltNames
  end

  methods
      function p = propertyEntry(name, type, id, numBytes, rw, dispType, eltNames)
      if nargin == 7
        p.name = name;
        p.type = type;
        p.id = id;
        p.numBytes = numBytes;
        p.rw = rw;
        p.dispType = dispType;
        p.eltNames = eltNames;
      end
    end
  end
end