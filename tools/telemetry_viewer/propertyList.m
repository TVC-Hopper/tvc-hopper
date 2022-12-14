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
              {"Sensors and Actuators", [6,7,8,9,10, 11]};...
              {"Tune Control Parameters", [12, 13, 14, 15, 16, 17, 18]}];
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

            pl.props = [propertyEntry('LAUNCH HOPPER',         'uint8',   @uint8,   0,   1,  'W', 'C', false, @pl.dummy,                  [0], [""]),...
                        propertyEntry('KILL EDF',              'uint8',   @uint8,   1,   1,  'W', 'C', false, @pl.dummy,                  [0], [""]),...
                        propertyEntry('RESET SYSTEM',          'uint8',   @uint8,  30,   1,  'W', 'C', false, @pl.dummy,                  [0], [""]),...
                        propertyEntry('HOPPER FLIGHT STATUS', 'uint32',  @uint32,   2,   4,  'R', 'S', false, @pl.dummy,                  [1], ["status: "], pl.statusIdsToStatusNames),...
                        propertyEntry('TARGET POSITION',      'single',  @single,  41,  12, 'RW', 'V', false, @pl.dummy,                  [3], ["xpos:", "ypos:", "zpos:"]),...
                        propertyEntry('BATTERY VOLTAGE',      'single',  @single,  18,   4,  'R', 'V', false, @pl.dummy,                  [1], ["volts:"]),...
                        propertyEntry('ESC THROTTLE',         'single',  @single,  53,   4, 'RW', 'V', false, @pl.dummy,                  [1], ["Pulse Width"]), ...
                        propertyEntry('SERVO POSITIONS',      'single',  @single,  40,  16, 'RW', 'V', false, @pl.dummy,                  [4], ["servo 1:","servo 2:","servo 3:","servo 4:"]), ...
                        propertyEntry('RAW LIDAR',            'uint32',  @uint32,  50,   4,  'R', 'V', false, @pl.dummy,                  [1], ["altitude"]),...
                        propertyEntry('PROCESSED IMU',        'single',  @single,  52,  24,  'R', 'V', false, @pl.dummy,                [3 3], ["Roll", "Pitch", "Yaw", "vRoll", "vPitch", "vYaw"]),...
                        propertyEntry('RAW IMU',              'single',  @single,  51,  40,  'R', 'V', false, @pl.dummy,              [3 3 4], ["accX", "accY", "accZ", "gyroX", "gyroY", "gyroZ", "?", "?", "?", "?"]),...
                        propertyEntry('Z LIMIT SPINUP',       'single',  @single,  55,   4,  'W', 'V', false, @pl.dummy,                  [1], ["z error spinup limit"]), ...
                        propertyEntry('Z LIMIT NORMAL',       'single',  @single,  56,   4,  'W', 'V', false, @pl.dummy,                  [1], ["z error normal limit"]), ...
                        propertyEntry('MAX Z INT',            'single',  @single,  57,   4,  'W', 'V', false, @pl.dummy,                  [1], ["max zerror integral"]), ...
                        propertyEntry('MAX ESC OUTPUT',       'single',  @single,  58,   4,  'W', 'V', false, @pl.dummy,                  [1], ["max esc pulse width"]), ...
                        propertyEntry('SET PITCH OFFSET',     'single',  @single,  59,   4,  'W', 'V', false, @pl.dummy,                  [1], ["pitch offset"]), ...
                        propertyEntry('SET ROLL OFFSET',      'single',  @single,  60,   4,  'W', 'V', false, @pl.dummy,                  [1], ["roll offset"]), ...
                        propertyEntry('K MATRIX',             'single',  @single,  54,  40, 'RW', 'M',  true, @pl.processKmatrix, [9 9 9 9 9], ["roll", "pitch", "yaw", "angVelx", "angVely", "angVelz", "zpos", "zvel", "integral"])];
        end
        
        function dummy()
        end

        function processKmatrix(a,editChildren)
            lqrGui("Tune K Matrix", editChildren{1});
        end
    end
end