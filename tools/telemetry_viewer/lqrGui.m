classdef lqrGui < baseGui
    properties
        Q = [     .001         0         0         0         0         0         0         0         0;
                     0      .001         0         0         0         0         0         0         0;
                     0         0      .001         0         0         0         0         0         0;
                     0         0         0      .001         0         0         0         0         0;
                     0         0         0         0      .001         0         0         0         0;
                     0         0         0         0         0      .001         0         0         0;
                     0         0         0         0         0         0      .001         0         0;
                     0         0         0         0         0         0         0      .001         0;
                     0         0         0         0         0         0         0         0        .01];
        
        R = [0.0100         0         0         0         0;
                  0    0.0100         0         0         0;
                  0         0    0.0100         0         0;
                  0         0         0    0.0100         0;
                  0         0         0         0    1.0000];

        sysStateSpace
        K
        kEditChildren

        qNames = ["Roll", "Pitch", "Yaw", "AngVelX","AngVelY","AngVelZ","Zpos","Zvel", "IntegralGain"];
        qEditChildren;
        rNames = ["Attack Angle 1", "Attack Angle 2", "Attack Angle 3", "Attack Angle 4", "Fan Speed"];
        rEditChildren;

        updateButtonName = ["Compute K"];
        updateButton; 

        tempGlob
    end
    methods
        function obj = lqrGui(name, editChildren)
            obj = obj@baseGui(5, 40, name, 400, 100);
            %obj.sysStateSpace = ss(obj.A, obj.B, obj.C, obj.D);
            obj.initChildren();
            obj.recomputeSys();
            obj.K = lqr(obj.sysStateSpace, obj.Q, obj.R);
            obj.kEditChildren = editChildren;
            obj.updateKvals();
            obj.makeGuiFig();
            obj.addQRTuningFields();
        end

        function initChildren(obj)
            fig = uifigure('visible', 'off');
            obj.qEditChildren = uibutton(fig).empty(1, 0);
            obj.rEditChildren = uibutton(fig).empty(1, 0);
            obj.kEditChildren = uibutton(fig).empty(1, 0);
            delete(fig);
        end

        function width = computeFigWidth(obj)            
            width = max(arrayfun(@obj.computeWidth, obj.qNames));
            width = width * 9 + 10 * obj.borderWidth;
        end 

        function height = computeFigHeight(obj)
            height = 7 * obj.defEltHeight + 8 * obj.borderWidth;
        end

        function computeK(obj, ~)
            obj.updateQvals();
            obj.updateRvals();
            obj.K = lqr(obj.sysStateSpace, obj.Q, obj.R);
            obj.updateKvals();
        end

        function updateQvals(obj)
            for i = 9:-1:1
                q(i) = obj.qEditChildren(i).Value;
            end
            obj.Q = diag(q);
        end

        function updateRvals(obj)
            for i = 5:-1:1
                r(i) = obj.rEditChildren(i).Value;
            end
            obj.R = diag(r);
        end

        function updateKvals(obj)
            for i = 1:5
                for j = 1:9
                    obj.kEditChildren((i-1)*9 + j).Value = obj.K(i, j);
                end
            end
        end

        function recomputeSys(obj, ~)
            obj.sysStateSpace = getStateSpaceRep();
        end

        function addQRTuningFields(obj)
            bgcolor1 = [0.0863    0.9608    0.8157];%[0.2235 0.8902 0.7216];
            txtcolor = [.1, .1, .1];
            label1 = "Q Matrix Diagonal";
            label2 = "R Matrix Diagonal";
            width = obj.figWidth - 2 * obj.borderWidth;
            obj.makeUiLabel(label1, [obj.currXpos, obj.currYpos, width, obj.defEltHeight], 'center', bgcolor1, txtcolor);
            obj.moveYposDown();
            
            fieldType = 'numeric';
            
            widthLabel = max(arrayfun(@obj.computeWidth, obj.qNames));    
            obj.tempGlob = widthLabel;
            arrayfun(@obj.makeLabelStrip, obj.qNames);
            obj.moveYposDown();
            obj.resetXposLeft();
            
            qVals = diag(obj.Q);
            for i = 1:size(obj.qNames, 2)
                child = obj.makeUiEditField(fieldType, qVals(i), [.9, .9, .9], [.1, .1, .1], ...
                                                   [obj.currXpos, obj.currYpos, widthLabel, obj.defEltHeight]);
                obj.qEditChildren(i) = child;
                obj.moveXposRight(widthLabel);
            end
            
            obj.moveYposDown();
            obj.resetXposLeft();
            
            obj.makeUiLabel(label2, [obj.currXpos, obj.currYpos, width, obj.defEltHeight], 'center', bgcolor1, txtcolor);
            obj.moveYposDown();

            widthLabel = max(arrayfun(@obj.computeWidth, obj.rNames));    
            obj.tempGlob = widthLabel;
            arrayfun(@obj.makeLabelStrip, obj.rNames);
            obj.moveYposDown();
            obj.resetXposLeft();
            rVals = diag(obj.R);
            for i = 1:size(obj.rNames, 2)
                child = obj.makeUiEditField(fieldType, rVals(i), [.9, .9, .9], [.1, .1, .1], ...
                                                   [obj.currXpos, obj.currYpos, widthLabel, obj.defEltHeight]);
                obj.rEditChildren(i) = child;
                obj.moveXposRight(widthLabel);
            end
            
            obj.moveYposDown();
            obj.resetXposLeft();

            width = obj.computeWidth(obj.updateButtonName(1));
            obj.updateButton = obj.makeUiButton(obj.updateButtonName(1), [obj.currXpos, obj.currYpos, width, obj.defEltHeight],...
                         [.96, .5, .5], [1, 1, 1], @obj.computeK, {});
            obj.moveXposRight(width);
            width = obj.computeWidth("Recompute State Space Rep");
            obj.updateButton = obj.makeUiButton("Recompute State Space Rep", [obj.currXpos, obj.currYpos, width, obj.defEltHeight],...
                         [.5, .5, .8], [1, 1, 1], @obj.recomputeSys, {});
            
        end
        function width = computeWidth(obj, name)
              width = strlength(name) * 8;
        end

        function makeLabelStrip(obj, name)
            obj.makeUiLabel(name, [obj.currXpos, obj.currYpos, obj.tempGlob, obj.defEltHeight],...
                                       'center', [1 1 1], [0.1 0.1 0.1]);
            obj.moveXposRight(obj.tempGlob);
        end
    end
end