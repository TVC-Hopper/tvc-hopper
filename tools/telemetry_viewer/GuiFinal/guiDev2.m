classdef guiDev2 < handle
    properties
        tvc
        propertyList
        
        borderWidth = 5;
        
        pg
        ip
        port
        guiFig
        figWidth
        figHeight
        currYpos
        currXpos
        
        
        numButtons
        buttonWidth
        buttonHeight = 40;
        titleBarHeight = 40;

    end
    methods
        function g = guiDev2(ip, port)
            g.ip = ip;
            g.port = port;
            g.propertyList = propertyList();
            
            g.numButtons = g.getNumButtons();
            g.buttonWidth = g.getButtonWidth();
            g.figWidth = g.getGuiWidth();
            g.figHeight = g.getGuiHeight();

            g.resetYposUp();
            g.resetXposLeft();

            g.guiFig = uifigure('Name', 'Get/Set Properties',...
                                'Position', [100, 100, g.figWidth, g.figHeight]);
            g.makeButtons();
        end

        function moveYposDown(g)
            g.currYpos = g.currYpos - (g.borderWidth + g.buttonHeight);
        end

        function resetYposUp(g)
            g.currYpos = g.figHeight - g.borderWidth - g.titleBarHeight;
        end

        function resetXposLeft(g)
            g.currXpos = g.borderWidth;
        end

        function width = getButtonWidth(g)
           width = max(cellfun(@strlength, g.propertyList.groups(1:3))) * 7;
        end

        function width = getGuiWidth(g)
           width = max(cellfun(@strlength, g.propertyList.groups(1:3))) * 7 + 2 * g.borderWidth;
        end

        function height = getGuiHeight(g)
           height = size(g.propertyList.groups, 1) * g.buttonHeight + g.borderWidth * (g.numButtons + 1);
        end

        function numButtons = getNumButtons(g)
           numButtons = size(g.propertyList.groups, 1);
        end

        function makeButtons(g)
             for i = 1:g.numButtons 
                uibutton(g.guiFig, 'push', 'Text', g.propertyList.groups{i,1}, ...
                        'Position', [g.currXpos, g.currYpos, g.buttonWidth, g.buttonHeight], ...
                        'ButtonPushedFcn', @(btn,event) g.showGroup(i));
                g.moveYposDown();
            end
        end

        function showGroup(g, idx)
            y = g.propertyList.groups{idx,2};
            x = propertyEntry.empty(size(y,2),0);
            for j = 1:size(y,2)
                x(j) = g.propertyList.props(y(j));
            end 
            propertyGui(g.propertyList.groups{idx,1}, x, g.ip, g.port);
        end
    end
end