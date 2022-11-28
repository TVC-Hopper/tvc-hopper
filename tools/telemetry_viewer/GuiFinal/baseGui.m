classdef baseGui < handle
    properties
        figHeight
        figWidth
        borderWidth
        defEltHeight
        currYpos = 0;
        currXpos = 0;

        guiFig
        baseFigName
        fromBLdist 
        fromBRdist
        titleBarHeight = 40;

        editChildren
        buttonChildren
        labelChildren

    end
    
    methods (Abstract)
        height = computeFigHeight(obj)
        width = computeFigWidth(obj)
    end

    methods
        function obj = baseGui(borderWidth, defEltHeight, baseFigName, fromBLdist, fromBRdist)
            obj.defEltHeight = defEltHeight;
            obj.borderWidth = borderWidth;
            obj.baseFigName = baseFigName;
            obj.fromBLdist = fromBLdist;
            obj.fromBRdist = fromBRdist;
            obj.initChildArrays();
        end

        function initChildArrays(obj)
            fig = uifigure('visible', 'off');
            obj.buttonChildren = uibutton(fig).empty(1, 0);
            obj.labelChildren = uilabel(fig).empty(1, 0);
            obj.editChildren = uieditfield(fig).empty(1, 0);
            delete(fig);
        end

        function makeGuiFig(obj)
            obj.figHeight = obj.computeFigHeight();
            obj.figWidth = obj.computeFigWidth();
            obj.guiFig = uifigure('Name', obj.baseFigName,...
                                  'Position', [obj.fromBLdist, obj.fromBRdist, obj.figWidth, obj.figHeight]);
            obj.resetYposUp();
            obj.resetXposLeft();
        end

        function button = makeUiButton(obj, name, position, bgcolor, txtcolor, func, args)
            button = uibutton(obj.guiFig, 'push', 'text', name, ...
                             'BackgroundColor', bgcolor,...
                             'FontColor', txtcolor,...
                             'Position', position, ...
                             'ButtonPushedFcn', @(btn,event) func(args));
            obj.buttonChildren(end + 1) = button;
        end

        function label = makeUiLabel(obj, name, position, alignment, bgcolor, txtcolor)
            label = uilabel(obj.guiFig,...
                           'Text', name,... 
                           'BackgroundColor',bgcolor, ...
                           'FontColor', txtcolor,...
                           'HorizontalAlignment', alignment,...
                           'FontSize',14, 'FontWeight', 'bold', 'FontColor', txtcolor, ...
                           'Position', position);
            obj.labelChildren(end + 1) = label;
        end

        function editField = makeUiEditField(obj, inputType, value, bgcolor, txtcolor, position)
            editField = uieditfield(obj.guiFig, inputType,...
                                'Value', value,...
                                'BackgroundColor',bgcolor, ...
                                'FontColor', txtcolor,...
                                'Position', position);
            if inputType == "numeric"
                editField.ValueDisplayFormat = '%.4f';
            end

            obj.editChildren(end + 1) = editField;
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
            obj.currYpos = obj.figHeight - obj.borderWidth - obj.titleBarHeight;
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
    