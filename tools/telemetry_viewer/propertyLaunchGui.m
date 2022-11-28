classdef propertyLaunchGui < baseGui
    properties
        propertyList

        numButtons
        maxButtonWidth
        ip
        port
    end

    methods
        function obj = propertyLaunchGui(ip, port)
            obj = obj@baseGui(20, 40, "Show Property Groups", 100, 100);
            
            obj.propertyList = propertyList();
            
            obj.numButtons = obj.propertyList.numGroups;
            obj.computeMaxButtonWidth();
            obj.makeGuiFig();

            obj.ip = ip;
            obj.port = port;
            obj.makeButtons();
        end

        function height = computeFigHeight(obj)
            height = obj.numButtons * obj.defEltHeight + obj.borderWidth * (obj.numButtons + 1);
        end

        function width = computeFigWidth(obj)
            width = obj.maxButtonWidth + 2 * obj.borderWidth;
        end

        function computeMaxButtonWidth(obj)
           obj.maxButtonWidth = max(cellfun(@strlength, obj.propertyList.groups(1:3))) * 7;
        end

        function makeButtons(obj)
            for i = 1:obj.numButtons
                name = obj.propertyList.groups{i,1};
                position = [obj.currXpos, obj.currYpos, obj.maxButtonWidth, obj.defEltHeight];
                bgcolor = 'white';
                txtcolor = 'black';
                obj.makeUiButton(name, position, bgcolor, txtcolor, @obj.showGroup, {i});
                obj.moveYposDown();
            end
        end
        

        function showGroup(obj, args)
            propIndices = obj.propertyList.groups{args{1},2};
            props = propertyEntry.empty(size(propIndices,2),0);
            for j = 1:size(propIndices,2)
                props(j) = obj.propertyList.props(propIndices(j));
            end 
            %disp(obj.propertyList.groups{args{1},1});
            color = [.98, .98, .98];
            for i = 1:20
                color(1, 2:3) = color(1, 2:3) - [.025, .025];
                if color(2) > 0 && color(3) > 0
                   obj.buttonChildren(args{1}).BackgroundColor = color;
                   pause(.005);
                end
           end

           propertyGui(obj.propertyList.groups{args{1},1}, props, obj.ip, obj.port);
           
           for i = 1:20
                color(1, 2:3) = color(1, 2:3) + [.025, .025];
                if color(2) < 1 && color(3) < 1
                   obj.buttonChildren(args{1}).BackgroundColor = color;
                   pause(.025);
                end
            end
        end
    end
end
       