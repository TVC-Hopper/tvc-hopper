classdef launchVehicleComms < baseGui
    properties
%         ip
%         port
        
        tvc
        tv
        pv

        numButtons = 2;
        buttonWidth
        buttonNames = ["Launch Telemtry Viewer", "Launch Vehicle Property Viewer"]
    end
    methods
        function obj = launchVehicleComms(ip, port)
            obj = obj@baseGui(10, 40, "Vehicle Comms", 100, 100);
%             obj.ip = ip;
%             obj.port = port;
            obj.tvc = TelemetryViewerClient(ip, port);
            obj.makeGuiFig();
            idxs = 1:2;
            arrayfun(@obj.makeVertButtonStrip, idxs);
        end

        function makeVertButtonStrip(obj, idx)
            bgc1 = [.96, .5, .5];
            txtc1 = [1,1,1];
            bgc2 = [.96, .5, .5];
            txtc2 = [1,1,1];
            bgcols = [bgc1; bgc2];
            txtcols = [txtc1; txtc2];
            funcs = {@obj.launchTelemtry, @obj.launchProperties};
            obj.makeUiButton(obj.buttonNames(idx), [obj.currXpos, obj.currYpos, obj.buttonWidth, obj.defEltHeight], ...
                         bgcols(idx, 1:3), txtcols(idx, 1:3), funcs{idx}, {});
            obj.moveYposDown();
            obj.resetXposLeft();
        end
        
        function height = computeFigHeight(obj)
            height = obj.numButtons * obj.defEltHeight + obj.borderWidth * (obj.numButtons + 1);
        end

        function width = computeFigWidth(obj)
            obj.buttonWidth = max(arrayfun(@strlength, obj.buttonNames)) * 10;
            width = obj.buttonWidth + 2 * obj.borderWidth;
        end

        function launchTelemtry(obj, ~)
            obj.tv = TelemetryApp(obj.tvc); 
        end

        function launchProperties(obj, ~)
            obj.pv = propertyLaunchGui(obj.tvc);
        end
    end
end