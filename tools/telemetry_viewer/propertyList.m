classdef propertyList
    properties
    typeNames = ["uint16", "uint32", "int16", "int32", "float", "bool", "array", "command"];
    typeIDs = [hex2dec('E0'), hex2dec('E1'), hex2dec('E2'), hex2dec('E3'), hex2dec('E4'), hex2dec('E5'), hex2dec('E6'), hex2dec('E7')];
    typeNameToTypeId;
    typeIdToTypeName;

    permissions = [1,2,4,8];
    chars = ["R", "W", "RS", "WS"];
    permissionsToChars;
    charsToPermissions;

    statusNames = ["stationary", "takeoff", "in flight", "landing close", "landing far"];
    statusIds = [0, 1, 2, 3, 4];
    statusIdsToStatusNames;
    
    props = [propertyEntry('launch hopper',        'bool',    0,  1,  'W', 'C',     [0], [""]),...
             propertyEntry('kill edf',             'bool',    1,  1,  'W', 'C',     [0], [""]),...
             propertyEntry('reset system',         'bool',   30,  1,  'W', 'C',     [0], [""]),...
             propertyEntry('hopper flight status', 'uint32',  2,  4,  'R', 'S',     [1], ["status:"]),...
             propertyEntry('target position',      'float',  41, 12, 'RW', 'A',     [3], ["xpos:", "ypos:", "zpos:"]),...
             propertyEntry('battery voltage',      'float',  18,  4,  'R', 'N',     [1], ["volts:"]),...
             propertyEntry('servo positions',      'float',  40, 16, 'RW', 'A',     [4], ["servo 1:","servo 2:","servo 3:","servo 4:"]), ...
             propertyEntry('raw lidar',            'uint32', 50,  4,  'R', 'N',     [1], ["altitude"]),...
             propertyEntry('raw Imu',              'float',  51, 40,  'R', 'A', [3 3 4], ["accX", "accY", "accZ", "gyroX", "gyroY", "gyroZ", "?", "?", "?", "?"])...
             propertyEntry('K matrix',             'float',  50,  4, 'RW', 'M',   [5 9], ["roll", "pitch", "yaw", "angVelx", "angVely", "angVelz", "zpos", "zvel"])];

    groups = [{"Command and Control", [1,2,3,4,5]};...
              {"Sensors and Actuators", [6,7,8,9]};...
              {"K Matrix Tuning", [10]}];

    end
    methods
        function pl = propertyList()
            pl.typeNameToTypeId = containers.Map(pl.typeNames, pl.typeIDs);
            pl.typeIdToTypeName = containers.Map(pl.typeIDs, pl.typeNames);
            pl.permissionsToChars = containers.Map(pl.permissions, pl.chars);
            pl.charsToPermissions = containers.Map(pl.chars, pl.permissions);
            pl.statusIdsToStatusNames = containers.Map(pl.statusIds, pl.statusNames);
        end

        function id = extractId(pl)
        end
        function type = extractType(pl)
        end
        function size = extractSize(pl)
        end
        function flags = extractFlags(pl)
        end
        function name = extractName(pl)
        end
    end
end