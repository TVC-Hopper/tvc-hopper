classdef propertyLaunchGui < baseGui
    properties
    end
    methods
        function obj = propertyLaunchGui(borderWidth, defaultEltHeight)
            obj = obj@baseGui(borderWidth, defaultEltHeight);
            obj.computeFigHeight();
            obj.computeFigWidth();
        end

        function computeFigHeight(obj)
        end

        function computeFigWidth(obj)
        end

        function dispbw(gb,val)
            %gb.borderWidth = val;
            disp(obj.borderWidth);
        end

    end
end
       