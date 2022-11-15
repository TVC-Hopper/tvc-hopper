classdef guiDev2 < handle
    properties
        tvc
        propertyList = [propertyEntry('Servo Positions', 'Array', 40, 16, 'RW', 'A', ["Servo 1:","Servo 2:","Servo 3:","Servo 4:"]), ...
                        propertyEntry('Raw Lidar', 'U32', 50, 1, 'R','S', [0])];
        
        borderWidth = 5;
        
        pg
        ip
        port
        guiFig
        figWidth
        figHeight
        
        
        numButtons
        buttonWidth
        buttonHeight = 40;

    end
    methods
        function g = guiDev2(ip, port)
            g.ip = ip;
            g.port = port;
            maxNameSize = 0;
            for i = 1:size(g.propertyList,2)
                nameSize = size(g.propertyList(i).name, 2);
                if nameSize > maxNameSize
                    maxNameSize = nameSize;
                end
            end
            g.numButtons = size(g.propertyList, 2);
            g.buttonWidth = 7 * maxNameSize;

            g.figWidth = g.buttonWidth + g.borderWidth * 2;
            g.figHeight = g.buttonHeight * g.numButtons + g.borderWidth * (g.numButtons + 1);

            g.guiFig = uifigure('Name', 'Get/Set Properties',...
                                'Position', [100 100 g.figWidth g.figHeight]);
            for i = 1:g.numButtons 
                uibutton(g.guiFig, 'push', 'Text', g.propertyList(i).name, ...
                        'Position', [g.borderWidth, g.borderWidth * i + (i-1) * g.buttonHeight, g.buttonWidth, g.buttonHeight], ...
                        'ButtonPushedFcn', @(btn,event) g.showProperty(i));
            end
        end

        function showProperty(g, idx)
            g.pg = propertyGui(g.propertyList(idx), g.ip, g.port);
        end
    end
end