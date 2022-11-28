classdef propertyGui < handle
 properties
        propList
        numProps
        ip
        port
        tvc
        dataBuf

        borderWidth = 5;
        
        guiFig
        figWidth
        figHeight
        figTitleHeight = 40;

        currYpos = 0;
        currXpos = 0;

        eltHeight = 40;
        widthInputField = 60;
        numRows = 0;
        numDatum = 0;
        numChildren = 0;
        uiEditChildren
        uiButtonChildren

        buttonLabelText = ["Set Direction:", "Get/Set:"];
        buttonNames = ["Read", "Write", "Update"];
        pushedColor = [0.7059 0.9294 0.8745];
        unPushedColor = [0.96 0.96 0.96];
        numButtons = 3;
        read = 1;
        write = 2;
        update = 3;
        propertyStateArray

        % 1: read, 2: write
        currState
        stateCanChange
    end
    methods
        function p = propertyGui(name, propList, ip, port)
            p.propList = propList;
            p.ip = ip;
            p.port = port;
            %p.tvc = TelemetryViewerClient(ip, port);
            p.numProps = size(p.propList, 2);
            
%             p.computeNumEltsNumDatum();
%             p.computeUiEditChildrenIdxs();
%             p.computeUiButtonChildrenIdxs();

            p.figWidth = p.computeFigWidth();
            p.figHeight = p.computeFigHeight();
            p.guiFig = uifigure('Name', name, 'Position', [100, 100, p.figWidth, p.figHeight + 40]);
            
            p.resetYposUp();
            p.resetXposLeft();
            
            p.makePropertyStateArray();
            
            for i = 1:p.numProps
                p.makeTitleType(i, [0.2235 0.8902 0.7216], [0.4431 0.9608 0.8314]);
                p.makeButtons(i);
                p.makeUserInputFields(i);
            end 

            p.initButtonState();
        end

        function moveYposDown(p, ~)
            p.currYpos = p.currYpos - (p.borderWidth + p.eltHeight);
        end

        function moveYposUp(p, ~)
            p.currYpos = p.currYpos + (p.borderWidth + p.eltHeight);
        end

        function resetYposUp(p, ~)
            p.currYpos = p.figHeight - p.borderWidth;
        end

        function resetYposDown(p, ~)
            p.currYpos = p.borderWidth;
        end

        function moveXposRight(p, moveRightAmt)
            p.currXpos = p.currXpos + p.borderWidth + moveRightAmt;
        end

        function moveXposLeft(p, moveLeftAmt)
            p.currXpos = p.currXpos - (p.borderWidth + moveLeftAmt);
        end

        function resetXposLeft(p, ~)
            p.currXpos = p.borderWidth;
        end

        function resetXposRight(p, ~)
            p.currXpos = p.figWidth - p.borderWidth;
        end

        function maxWidth = computeFigWidth(p)
            maxWidth = max([p.computeButtonStripWidth(), p.computeMaxInputRowWidth()]);
        end

        function makePropertyStateArray(p)
            p.propertyStateArray = propertyState.empty(p.numProps, 0);
            for i = 1:p.numProps
                p.propertyStateArray(i) = propertyState(p.propList(i));
            end
        end

        function width = computeMaxInputRowWidth(p)
            max = 0;
            for i = 1:p.numProps
                startIdx = 1;
                for j = 1:size(p.propList(i).dispDims, 2) 
                    numNamesInRow = p.propList(i).dispDims(j);
                    endIdx = startIdx + numNamesInRow - 1;
                    rowNames = p.propList(i).eltNames(startIdx:endIdx);
                    sumNameLengths = sum(arrayfun(@strlength,rowNames));
                    totalLength = sumNameLengths * 10 + numNamesInRow * p.widthInputField + (numNamesInRow * 2 + 1) * p.borderWidth;
                    if max < totalLength
                        max = totalLength;
                    end
                startIdx = endIdx + 1;
                end
                
            end
            width = max;
        end

        function width = computeButtonStripWidth(p)
            names = [p.buttonLabelText, p.buttonNames];
            width = sum(arrayfun(@strlength, names)) * 10 + (size(names, 2) + 1)* p.borderWidth;
        end

        function numRows = computeFigHeight(p, ~)
            numRows = sum(arrayfun(@p.computeNumRowsInProp, p.propList)) * (p.eltHeight + p.borderWidth + 1);
        end

        function numRows = computeNumRowsInProp(p, pe)
            if pe.dispDims(1) == 0
                numRows = 3;
            else
                numRows = 3 + size(pe.dispDims,2);
            end
        end

        function initButtonState(p)
            arr = flip(1:p.numChildren);
            for i=1:p.numProps
                p.propertyStateArray(i).initChildren(arr);
                if p.propertyStateArray(i).property.rw == 'RW'
                    p.propertyStateArray(i).stateCanChange = 1;
                    p.propertyStateArray(i).currState = p.read;
                    p.guiFig.Children(p.propertyStateArray(i).uiButtonChildren(p.read)).BackgroundColor = p.pushedColor;
                elseif p.propertyStateArray(i).property.rw == 'R'
                    p.propertyStateArray(i).stateCanChange = 0;
                    p.propertyStateArray(i).currState = p.read;
                    p.guiFig.Children(p.propertyStateArray(i).uiButtonChildren(p.read)).BackgroundColor = p.pushedColor;
                else
                    p.propertyStateArray(i).stateCanChange = 0;
                    p.propertyStateArray(i).currState = p.write;
                    p.guiFig.Children(p.propertyStateArray(i).uiButtonChildren(p.write)).BackgroundColor = p.pushedColor;
                end
            end
        end

        function computeUiEditChildrenIdxs(p, ~)
            p.uiEditChildren = flip(1:2:(2 * p.numDatum - 1));
        end

        function computeUiButtonChildrenIdxs(p, ~)
            offset = 2 * p.numDatum;
            p.uiButtonChildren = flip([offset + 1, offset + 3, offset + 4]);
        end

        function computeNumEltsNumDatum(p, ~)
            for i = 1:size(p.propList,2)
                p.numRows = p.numRows + 3;
                if p.propList(i).type == 'bool'
                    continue;
                elseif p.propList(i).type == 'uint32' || p.propList(i).type == 'float'
                    
                end
            end
%             numElts = 1;
%             if p.propList.type == 'array'
%                 numElts = p.propList.numBytes / 4;
%             end
%             p.numDatum = numElts;
%             p.numElts = numElts + 3;
        end

        function width = computeWidth(p, name)
              width = strlength(name) * 10;
        end

        function makeTitleType(p, i, color1, color2)
            name = append('Name: ', p.propList(i).name);
            type = append('Type: ', p.propList(i).type);
            width = p.figWidth - 2 * p.borderWidth;
            p.makeUiLabel(name, [p.currXpos, p.currYpos, width, p.eltHeight], 'center', color1);
            p.incrementChildCount();
            p.moveYposDown();
            p.makeUiLabel(type, [p.currXpos, p.currYpos, width, p.eltHeight], 'center', color2);
            p.incrementChildCount();
            p.moveYposDown();
        end

        function p = incrementChildCount(p)
            p.numChildren = p.numChildren + 1;
        end

        function makeButtons(p, i)
            labelText = "Set Direction:";
            width = p.computeWidth(labelText);
            p.makeUiLabel(labelText, [p.currXpos, p.currYpos, width, p.eltHeight], 'center', [0.9608    0.4902    0.6314]);
            p.incrementChildCount();
            p.moveXposRight(width);

            width = p.computeWidth(p.buttonNames(p.read));
            p.makeUiButton(p.buttonNames(p.read), [p.currXpos, p.currYpos, width, p.eltHeight], p.read, i);
            p.incrementChildCount();
            p.propertyStateArray(i).addButtonChild(p.numChildren);
            p.moveXposRight(width);

            width = p.computeWidth(p.buttonNames(p.write));
            p.makeUiButton(p.buttonNames(p.write), [p.currXpos, p.currYpos, width, p.eltHeight], p.write, i);
            p.incrementChildCount();
            p.propertyStateArray(i).addButtonChild(p.numChildren);
            p.moveXposRight(width);

            labelText = "Get/Set:";
            width = p.computeWidth(labelText);
            p.makeUiLabel(labelText, [p.currXpos, p.currYpos, width, p.eltHeight], 'center', [0.9608    0.4902    0.6314]);
            p.incrementChildCount();
            p.moveXposRight(width);

            width = p.computeWidth(p.buttonNames(p.update));
            p.makeUiButton(p.buttonNames(p.update), [p.currXpos, p.currYpos, width, p.eltHeight], p.update, i);
            p.incrementChildCount();
            p.propertyStateArray(i).addButtonChild(p.numChildren);
            
            fillWidth = p.figWidth - p.borderWidth -p.currXpos;
            if fillWidth > 0
                fillLabelPos = [p.currXpos, p.currYpos, fillWidth, p.eltHeight];
                p.makeUiLabel("", fillLabelPos, 'center', [0.9608    0.4902    0.6314]);
                p.incrementChildCount();
            end
            
            p.resetXposLeft();
            p.moveYposDown();
        end

        function makeUserInputFields(p, idx)
            if p.propList(idx).dispDims(1) == 0
                return;
            end
            startIdx = 0;
            widthLabel = max(arrayfun(@p.computeWidth, p.propList(idx).eltNames));
            for i = 1:size(p.propList(idx).dispDims, 2)
                %labelText = strcat("[", num2str(i), "]");
                for j = 1:p.propList(idx).dispDims(i)
                    labelText = p.propList(idx).eltNames(startIdx + j);
                    disp(labelText)
                    %widthLabel = p.computeWidth(labelText);
                    p.makeUiLabel(labelText, [p.currXpos, p.currYpos, widthLabel, p.eltHeight], 'center', [0.9608    0.6706    0.8510]);
                    p.incrementChildCount();
                    p.moveXposRight(widthLabel);
                    p.makeUiEditField([p.currXpos, p.currYpos, p.widthInputField, p.eltHeight]);
                    p.moveXposRight(p.widthInputField);
                    p.incrementChildCount();
                    p.propertyStateArray(idx).addEditChild(p.numChildren);
                end
                startIdx = startIdx + p.propList(idx).dispDims(i);
                fillWidth = p.figWidth - p.borderWidth -p.currXpos;
                if fillWidth > 0
                    fillLabelPos = [p.currXpos, p.currYpos, fillWidth, p.eltHeight];
                    p.makeUiLabel("", fillLabelPos, 'center', [0.9608    0.6706    0.8510]);
                    p.incrementChildCount();
                end
                p.moveYposDown();
                p.resetXposLeft();
            end
        end

        function makeUiLabel(p, name, position, alignment, bgcolor)
            color = [1 1 1];
            if nargin == 5
                color = bgcolor;
            end
            uilabel(p.guiFig,...
                   'Text', name,... 
                   'BackgroundColor',color, 'HorizontalAlignment', alignment,'FontSize',15, 'FontWeight', 'bold', 'FontColor', 'black', ...
                   'Position', position);
        end

        function makeUiButton(p, name, position, buttonIdx, propIdx)
            uibutton(p.guiFig, 'push', 'text', name, ...
                    'Position', position, ...
                    'ButtonPushedFcn', @(btn,event) p.buttonPushed(buttonIdx, propIdx));
        end

        function makeUiEditField(p,position)
            uieditfield(p.guiFig,'numeric',...
                       'Value', -inf,'Position', position);
        end

        function rx(p, propIdx)
            p.tvc.RequestProperty(p.propertyStateArray(propIdx).property.id);
            pause(0.1);
            p.tvc.RequestValue(p.propertyStateArray(propIdx).property.id);
            [id, sz, t, val] = p.tvc.ReadValue();
            p.dataBuf = typecast(uint8(val), 'single');
            for i = 1:p.propertyStateArray(propIdx).property.numElts
                p.guiFig.Children(p.guiFig.Children(p.propertyStateArray(propIdx).uiEditChildren(i))).Value = double(p.dataBuf(i));
            end

        end

        function tx(p, propIdx)
            for i = 1:p.numDatum
                p.dataBuf(i) = single(p.guiFig.Children(p.propertyStateArray(propIdx).uiEditChildren(i)).Value);
            end
            txBuf = typecast(p.dataBuf, 'uint8');
            p.tvc.SetProperty(p.propertyStateArray(propIdx).property.id, txBuf);
        end

        function buttonPushed(p, buttonIdx, propIdx)
            if buttonIdx == p.read && p.propertyStateArray(propIdx).stateCanChange
                p.propertyStateArray(propIdx).currState = p.read;
                p.guiFig.Children(p.propertyStateArray(propIdx).uiButtonChildren(p.write)).BackgroundColor = p.unPushedColor;
                p.guiFig.Children(p.propertyStateArray(propIdx).uiButtonChildren(p.read)).BackgroundColor = p.pushedColor;   
            elseif buttonIdx == p.write && p.propertyStateArray(propIdx).stateCanChange
                p.propertyStateArray(propIdx).currState = p.write;
                p.guiFig.Children(p.propertyStateArray(propIdx).uiButtonChildren(p.read)).BackgroundColor = p.unPushedColor;
                p.guiFig.Children(p.propertyStateArray(propIdx).uiButtonChildren(p.write)).BackgroundColor = p.pushedColor;
            elseif buttonIdx == p.update
                if p.propertyStateArray(propIdx).currState == p.read
                    p.rx(propIdx);
                else
                    p.tx(propIdx);
                end
            end
        end
    end
end