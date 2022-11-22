classdef baseGui < handle
    properties
        figHeight
        figWidth
        borderWidth
        defEltHeight
        currYpos = 0;
        currXpos = 0;

        guiFig

    end
    
    methods (Abstract)
        computeFigHeight(obj)
        computeFigWidth(obj)
    end

    methods
        function obj = baseGui(borderWidth, defEltHeight, baseFigName, fromBLdist, fromBRdist)
            obj.defEltHeight = defEltHeight;
            obj.borderWidth = borderWidth;
            obj.figHeight = computeFigHeight();
            obj.figWidth = computeFigWidth();
            obj.guiFig = uifigure('Name', baseFigName,...
                                  'Position', [fromBLdist, fromBRdist, g.figWidth, g.figHeight]);
            obj.resetYposUp();
            obj.resetXposLeft();
        end

        function moveYposDown(obj, shift)
            yShiftAmt = obj.borderWidth + obj.defEltHeight;
            if (nargin == 2)
                yShiftAmt = obj.borderWidth + shift;
            end 
            obj.currYpos = obj.currYpos - yShiftAmt;
        end

        function moveYposUp(obj, shift)
            yShiftAmt = obj.borderWidth + obj.defEltHeight;
            if (nargin == 2)
                yShiftAmt = obj.borderWidth + shift;
            end 
            obj.currYpos = obj.currYpos + yShiftAmt;
        end

        function resetYposUp(obj)
            obj.currYpos = obj.figHeight - obj.borderWidth;
        end

        function resetYposDown(obj)
            obj.currYpos = obj.borderWidth;
        end

        function moveXposRight(obj, moveRightAmt)
            obj.currXpos = obj.currXpos + obj.borderWidth + moveRightAmt;
        end

        function moveXposLeft(obj, moveLeftAmt)
            obj.currXpos = obj.currXpos - (obj.borderWidth + moveLeftAmt);
        end

        function resetXposLeft(obj)
            obj.currXpos = obj.borderWidth;
        end

        function resetXposRight(obj)
            obj.currXpos = obj.figWidth - obj.borderWidth;
        end
    end
end
    