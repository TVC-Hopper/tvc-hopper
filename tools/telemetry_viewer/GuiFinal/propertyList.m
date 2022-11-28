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
    
    props

    groups = [{"Command and Control", [1,2,3,4,5]};...
              {"Sensors and Actuators", [6,7,8,9]};...
              {"K Matrix Tuning", [10]}];
    numGroups

    end
    methods
        function pl = propertyList()
            pl.typeNameToTypeId = containers.Map(pl.typeNames, pl.typeIDs);
            pl.typeIdToTypeName = containers.Map(pl.typeIDs, pl.typeNames);
            pl.permissionsToChars = containers.Map(pl.permissions, pl.chars);
            pl.charsToPermissions = containers.Map(pl.chars, pl.permissions);
            pl.statusIdsToStatusNames = containers.Map(pl.statusIds, pl.statusNames);
            pl.numGroups = size(pl.groups, 1);

            pl.props = [propertyEntry('launch hopper',         'uint8',  @uint8,   0,  1,  'W', 'C', false, @pl.dummy,                  [0], [""]),...
                        propertyEntry('kill edf',              'uint8',  @uint8,   1,  1,  'W', 'C', false, @pl.dummy,                  [0], [""]),...
                        propertyEntry('reset system',          'uint8',  @uint8,  30,  1,  'W', 'C', false, @pl.dummy,                  [0], [""]),...
                        propertyEntry('hopper flight status', 'uint32', @uint32,   2,  4,  'R', 'S', false, @pl.dummy,                  [1], ["status: "], pl.statusIdsToStatusNames),...
                        propertyEntry('target position',       'float',  @float,  41, 12, 'RW', 'V', false, @pl.dummy,                  [3], ["xpos:", "ypos:", "zpos:"]),...
                        propertyEntry('battery voltage',       'float',  @float,  18,  4,  'R', 'V', false, @pl.dummy,                  [1], ["volts:"]),...
                        propertyEntry('servo positions',       'float',  @float,  40, 16, 'RW', 'V', false, @pl.dummy,                  [4], ["servo 1:","servo 2:","servo 3:","servo 4:"]), ...
                        propertyEntry('raw lidar',            'uint32', @uint32,  50,  4,  'R', 'V', false, @pl.dummy,                  [1], ["altitude"]),...
                        propertyEntry('raw Imu',               'float',  @float,  51, 40,  'R', 'V', false, @pl.dummy,              [3 3 4], ["accX", "accY", "accZ", "gyroX", "gyroY", "gyroZ", "?", "?", "?", "?"])...
                        propertyEntry('K matrix',              'float',  @float,  50,  4, 'RW', 'M',  true, @pl.processKmatrix, [9 9 9 9 9], ["roll", "pitch", "yaw", "angVelx", "angVely", "angVelz", "zpos", "zvel", "integral"])];
        end
        
        function dummy()
        end

        function processKmatrix(a,editChildren)
            lqrGui("Tune K Matrix", editChildren{1});
        end
    end
end