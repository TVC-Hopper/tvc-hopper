classdef propertyGui < handle
 properties
        propertyParams 
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
        numElts
        numDatum
        uiEditChildren
        uiButtonChildren

        buttonNames = ["Read", "Write", "Update"];
        pushedColor = [0.7059 0.9294 0.8745];
        unPushedColor = [0.96 0.96 0.96];
        numButtons = 3;
        read = 1;
        write = 2;
        update = 3;

        % 1: read, 2: write
        currState
        stateCanChange
    end
    methods
        function p = propertyGui(property, ip, port)
            p.propertyParams = property;
            p.ip = ip;
            p.port = port;
            p.tvc = TelemetryViewerClient(ip, port);
            p.computeNumEltsDatum();
            p.computeUiEditChildrenIdxs();
            p.computeUiButtonChildrenIdxs();

            p.figWidth = 400;
            p.figHeight = p.numElts * (p.eltHeight + p.borderWidth + 1);
            p.guiFig = uifigure('Position', [100, 100, p.figWidth, p.figHeight + p.figTitleHeight]);
            
            p.resetYposUp();
            p.resetXposLeft();
            
            p.makeTitleType();
            p.makeButtons();
            p.makeUserInputFields();
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

        function initButtonState(p, ~)
            if p.propertyParams.rw == 'RW'
                p.stateCanChange = 1;
                p.currState = p.read;
                p.guiFig.Children(p.uiButtonChildren(1)).BackgroundColor = p.pushedColor;
            elseif p.propertyParams.rw == 'R'
                p.stateCanChange = 0;
                p.currState = p.read;
                p.guiFig.Children(p.uiButtonChildren(1)).BackgroundColor = p.pushedColor;
            else
                p.stateCanChange = 0;
                p.currState = p.read;
                p.guiFig.Children(p.uiButtonChildren(2)).BackgroundColor = p.pushedColor;
            end
        end

        function computeUiEditChildrenIdxs(p, ~)
            p.uiEditChildren = flip(1:2:(2 * p.numDatum - 1));
        end

        function computeUiButtonChildrenIdxs(p, ~)
            offset = 2 * p.numDatum;
            p.uiButtonChildren = flip([offset + 1, offset + 3, offset + 4]);
        end

        function computeNumEltsDatum(p, ~)
            numElts = 1;
            if p.propertyParams.type == 'Array'
                numElts = p.propertyParams.numBytes / 4;
            end
            p.numDatum = numElts;
            p.numElts = numElts + 3;
        end

        function width = computeWidth(p, name)
            width = strlength(name) * 10;
        end

        function makeTitleType(p, ~)
            name = append('Name: ', p.propertyParams.name);
            type = append('Type: ', p.propertyParams.type);
            width = p.figWidth - 2 * p.borderWidth;
            p.makeUiLabel(name, [p.currXpos, p.currYpos, width, p.eltHeight]);
            p.moveYposDown();
            p.makeUiLabel(type, [p.currXpos, p.currYpos, width, p.eltHeight]);
            p.moveYposDown();
        end

        function makeButtons(p, ~)
            labelText = "Set Direction:";
            width = p.computeWidth(labelText);
            p.makeUiLabel(labelText, [p.currXpos, p.currYpos, width, p.eltHeight]);
            p.moveXposRight(width);

            width = p.computeWidth(p.buttonNames(p.read));
            p.makeUiButton(p.buttonNames(p.read), [p.currXpos, p.currYpos, width, p.eltHeight], p.read);
            p.moveXposRight(width);

            width = p.computeWidth(p.buttonNames(p.write));
            p.makeUiButton(p.buttonNames(p.write), [p.currXpos, p.currYpos, width, p.eltHeight], p.write);
            p.moveXposRight(width);

            labelText = "Get/Set:";
            width = p.computeWidth(labelText);
            p.makeUiLabel(labelText, [p.currXpos, p.currYpos, width, p.eltHeight]);
            p.moveXposRight(width);

            width = p.computeWidth(p.buttonNames(p.update));
            p.makeUiButton(p.buttonNames(p.update), [p.currXpos, p.currYpos, width, p.eltHeight], p.update);
            p.resetXposLeft();
            p.moveYposDown();
        end

        function maxWidth = computeMaxWidth(p)
            lengths = arrayfun(@strlength, p.propertyParams.eltNames);
            maxWidth = max(lengths) * 10;
        end

        function makeUserInputFields(p)
            widthLabel = p.computeMaxWidth();
            widthInput = 60;
            for i = 1:p.numDatum
                %labelText = strcat("[", num2str(i), "]");
                labelText = p.propertyParams.eltNames(i);
                p.makeUiLabel(labelText, [p.currXpos, p.currYpos, widthLabel, p.eltHeight]);
                p.moveXposRight(widthLabel);
                p.makeUiEditField([p.currXpos, p.currYpos, widthInput, p.eltHeight]);
                p.moveYposDown();
                p.resetXposLeft();
            end
        end

        function makeUiLabel(p, name, position)
            uilabel(p.guiFig,...
                   'Text', name,... 
                   'BackgroundColor','white', 'HorizontalAlignment', 'center','FontSize',15, 'FontWeight', 'bold', 'FontColor', 'black', ...
                   'Position', position);
        end

        function makeUiButton(p, name, position, idx)
            uibutton(p.guiFig, 'push', 'text', name, ...
                    'Position', position, ...
                    'ButtonPushedFcn', @(btn,event) p.buttonPushed(idx));
        end

        function makeUiEditField(p,position)
            uieditfield(p.guiFig,'numeric',...
                       'Value', -inf,'Position', position);
        end

        function rx(p)
            p.tvc.RequestProperty(p.propertyParams.id);
            pause(0.1);
            p.tvc.RequestValue(p.propertyParams.id);
            [id, sz, t, val] = p.tvc.ReadValue();
            p.dataBuf = typecast(uint8(val), 'single');
            for i = 1:p.numDatum
                p.guiFig.Children(p.uiEditChildren(i)).Value = double(p.dataBuf(i));
            end

        end

        function tx(p)
            for i = 1:p.numDatum
                p.dataBuf(i) = single(p.guiFig.Children(p.uiEditChildren(i)).Value);
            end
            txBuf = typecast(p.dataBuf, 'uint8');
            p.tvc.SetProperty(p.propertyParams.id, txBuf);
        end

        function buttonPushed(p, buttonIdx)
            if buttonIdx == p.read && p.stateCanChange
                p.currState = p.read;
                p.guiFig.Children(p.uiButtonChildren(p.write)).BackgroundColor = p.unPushedColor;
                p.guiFig.Children(p.uiButtonChildren(p.read)).BackgroundColor = p.pushedColor;   
            elseif buttonIdx == p.write && p.stateCanChange
                p.currState = p.write;
                p.guiFig.Children(p.uiButtonChildren(p.read)).BackgroundColor = p.unPushedColor;
                p.guiFig.Children(p.uiButtonChildren(p.write)).BackgroundColor = p.pushedColor;
            elseif buttonIdx == p.update
                if p.currState == p.read
                    p.rx();
                else
                    p.tx();
                end
            end
        end
    end
end