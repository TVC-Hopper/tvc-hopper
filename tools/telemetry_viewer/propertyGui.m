classdef propertyGui < baseGui
    properties
        propertyList
        numProps
        
        ip
        port
        tvc
        
        dataBuf

        widthInputField = 60;

        buttonLabelText = ["Set Direction:", "Get/Set:"];
        buttonNames = ["Read", "Write", "Update"];

        propertyNameTextPre = ["", "Type: "];

        processDataButton
        processDataButtonTxt = "Process Property Data";
        
        pushedColor = [0.7059 0.9294 0.8745];
        unPushedColor = [0.96 0.96 0.96];
        read = 1;
        write = 2;
        update = 3;
        numButtons = 3;

        tempGlob

        propertyStateArray
    end

    methods
        function obj = propertyGui(name, propertyList, tvc)
            obj = obj@baseGui(5, 40, name, 400, 100);
            obj.propertyList = propertyList;
            obj.numProps = size(obj.propertyList, 2);
            
%             obj.ip = ip;
%             obj.port = port;
           % obj.tvc = TelemetryViewerClient(ip, port);
            obj.tvc = tvc;
            obj.makeGuiFig();
             
            obj.makePropertyStateArray();
            
            for i = 1:obj.numProps
                obj.makeTitleType(i);
                obj.makeButtons(i);
                obj.makeUserInputFields(i);
            end 

            obj.initButtonState();
        end

        function width = computeFigWidth(obj)
            names = [obj.buttonLabelText, obj.buttonNames];
            maxButtonStripWidth = sum(arrayfun(@strlength, names)) * 10 + (size(names, 2) + 1)* obj.borderWidth;
            maxRowLength = 0;
            for i = 1:obj.numProps
                propertyNameWidths(i) = sum(arrayfun(@strlength,[obj.propertyNameTextPre(1), obj.propertyList(i).name])) * 10;
                maxNameWidth = max(arrayfun(@obj.computeWidth, obj.propertyList(i).eltNames));
                maxEltsInRow = max(obj.propertyList(i).dispDims);
                switch obj.propertyList(i).dispType
                    case 'C'
                        fieldWidths = 0;
                        numElts = 0;
                    case 'S'
                        k = values(obj.propertyList(i).valToString);
                        maxInputFieldWidth = max(arrayfun(@obj.computeWidth, k));
                        fieldWidths = (maxNameWidth + maxInputFieldWidth) * maxEltsInRow;
                        numElts = maxEltsInRow * 2;
                    case 'V'
                        fieldWidths = (maxNameWidth + obj.widthInputField) * maxEltsInRow;
                        numElts = maxEltsInRow * 2;
                    case 'M'
                        fieldWidths = maxEltsInRow * max([obj.widthInputField, maxNameWidth]);
                        numElts = maxEltsInRow;
                    otherwise
                        disp("ERROR IN COMPUTEFIGWIDTH")
                end
                totalLength = fieldWidths + (numElts + 1) * obj.borderWidth;
                if maxRowLength < totalLength
                        maxRowLength = totalLength;
                end
            end
            width = max([maxButtonStripWidth, maxRowLength, max(propertyNameWidths)]);
        end 

        function numRows = computeNumRowsInProp(obj, pe)
            if pe.dispType == 'C'
                numRows = 3;
            elseif pe.dispType == 'M'
                numRows = 4 + size(pe.dispDims,2);
            else
                numRows = 3 + size(pe.dispDims,2);
            end
            if pe.processData
                numRows = numRows + 1;
            end
        end

        function height = computeFigHeight(obj)
            height = sum(arrayfun(@obj.computeNumRowsInProp, obj.propertyList)) * (obj.defEltHeight + obj.borderWidth + 1);
        end

        function makePropertyStateArray(obj)
            obj.propertyStateArray = propertyState.empty(obj.numProps, 0);
            for i = 1:obj.numProps
                obj.propertyStateArray(i) = propertyState(obj.propertyList(i));
            end
        end

        function makeTitleType(obj, i)
            bgcolor1 = [0.0863    0.9608    0.8157];%[0.2235 0.8902 0.7216];
            bgcolor2 = [0.4745    0.9294    0.8157];%[0.4431 0.9608 0.8314];
            txtcolor = [.1, .1, .1];
            name = append('', obj.propertyList(i).name);
            type = append('Type: ', obj.propertyList(i).type);
            width = obj.figWidth - 2 * obj.borderWidth;
            obj.makeUiLabel(name, [obj.currXpos, obj.currYpos, width, obj.defEltHeight], 'center', bgcolor1, txtcolor);
            obj.moveYposDown();
            obj.makeUiLabel(type, [obj.currXpos, obj.currYpos, width, obj.defEltHeight], 'center', bgcolor2, txtcolor);
            obj.moveYposDown();
        end

        function makeButtons(obj, i)
            labelcolor = [0.8314    0.9882    0.9490];
            labeltxtcolor = [.1, .1, .1];
            fillcolor = [0.8314    0.9882    0.9490];

            labelText = "";
            width = obj.computeWidth(labelText);
            obj.makeUiLabel(labelText, [obj.currXpos, obj.currYpos, width, obj.defEltHeight],...
                            'center', labelcolor, labeltxtcolor);
            obj.moveXposRight(width);

            width = obj.computeWidth(obj.buttonNames(obj.write));
            child = obj.makeUiButton(obj.buttonNames(obj.read), [obj.currXpos, obj.currYpos, width, obj.defEltHeight],...
                                     [.96, .96, .96], [.1, .1, .1], @obj.buttonPushed, {obj.read, i});
            obj.propertyStateArray(i).addButtonChild(child);
            obj.moveXposRight(width);

            width = obj.computeWidth(obj.buttonNames(obj.write));
            child = obj.makeUiButton(obj.buttonNames(obj.write), [obj.currXpos, obj.currYpos, width, obj.defEltHeight],...
                                     [.96, .96, .96], [.1, .1, .1], @obj.buttonPushed, {obj.write, i});
            obj.propertyStateArray(i).addButtonChild(child);
            obj.moveXposRight(width);

            labelText = " ";
            width = obj.computeWidth(labelText);
            obj.makeUiLabel(labelText, [obj.currXpos, obj.currYpos, width, obj.defEltHeight],...
                            'center', labelcolor, labeltxtcolor);
            obj.moveXposRight(width);

            width = obj.computeWidth(obj.buttonNames(obj.update));
            child = obj.makeUiButton(obj.buttonNames(obj.update), [obj.currXpos, obj.currYpos, width, obj.defEltHeight],...
                                     [.96, .5, .5], [1, 1, 1], @obj.buttonPushed, {obj.update, i});
            obj.propertyStateArray(i).addButtonChild(child)
            obj.moveXposRight(width);
            
            fillWidth = obj.figWidth - obj.borderWidth -obj.currXpos;
            
            if fillWidth > 0
                fillLabelPos = [obj.currXpos, obj.currYpos, fillWidth, obj.defEltHeight];
                obj.makeUiLabel("", fillLabelPos, 'center', fillcolor, [0,0,0]);
            end
            obj.resetXposLeft();
            obj.moveYposDown();
        end

        function initButtonState(obj)
            for i=1:obj.numProps
                if obj.propertyStateArray(i).property.rw == 'RW'
                    obj.propertyStateArray(i).stateCanChange = 1;
                    obj.propertyStateArray(i).currState = obj.read;
                    obj.propertyStateArray(i).buttonChildren(obj.read).BackgroundColor = obj.pushedColor;
                elseif obj.propertyStateArray(i).property.rw == 'R'
                    obj.propertyStateArray(i).stateCanChange = 0;
                    obj.propertyStateArray(i).currState = obj.read;
                    obj.propertyStateArray(i).buttonChildren(obj.read).BackgroundColor = obj.pushedColor;
                    obj.propertyStateArray(i).buttonChildren(obj.write).Enable = "off";
                else
                    obj.propertyStateArray(i).stateCanChange = 0;
                    obj.propertyStateArray(i).currState = obj.write;
                    obj.propertyStateArray(i).buttonChildren(obj.write).BackgroundColor = obj.pushedColor;
                    obj.propertyStateArray(i).buttonChildren(obj.read).Enable = "off";
                end
            end
        end

        function buttonPushed(obj, args)
            if args{1} == obj.read && obj.propertyStateArray(args{2}).stateCanChange
                obj.propertyStateArray(args{2}).currState = obj.read;
                obj.propertyStateArray(args{2}).buttonChildren(obj.write).BackgroundColor = obj.unPushedColor;
                obj.propertyStateArray(args{2}).buttonChildren(obj.read).BackgroundColor = obj.pushedColor;   
            elseif args{1} == obj.write && obj.propertyStateArray(args{2}).stateCanChange
                obj.propertyStateArray(args{2}).currState = obj.write;
                obj.propertyStateArray(args{2}).buttonChildren(obj.read).BackgroundColor = obj.unPushedColor;
                obj.propertyStateArray(args{2}).buttonChildren(obj.write).BackgroundColor = obj.pushedColor;
            elseif args{1} == obj.update
                if obj.propertyStateArray(args{2}).currState == obj.read
                    obj.rx(args{2});
                else
                    obj.tx(args{2});
                end
            end
        end

        function width = computeWidth(obj, name)
              width = strlength(name) * 8;
        end

        function makeLabelStrip(obj, name)
            obj.makeUiLabel(name, [obj.currXpos, obj.currYpos, obj.tempGlob, obj.defEltHeight],...
                                       'center', [1 1 1], [0.1 0.1 0.1]);
            obj.moveXposRight(obj.tempGlob);
        end

        function makeUserInputFields(obj, idx)
            if obj.propertyList(idx).dispDims(1) == 0
                return;
            end
            startIdx = 0;
            widthLabel = max(arrayfun(@obj.computeWidth, obj.propertyList(idx).eltNames));
            
            if obj.propertyList(idx).dispType  == 'M'
                obj.tempGlob = widthLabel;
                arrayfun(@obj.makeLabelStrip, obj.propertyList(idx).eltNames);
                obj.moveYposDown();
                obj.resetXposLeft();
            end
            
            for i = 1:size(obj.propertyList(idx).dispDims, 2)
                for j = 1:obj.propertyList(idx).dispDims(i)
                    fieldType = 'numeric';
                    initVal = -inf;
                    if obj.propertyList(idx).dispType == 'S'
                        fieldType = 'text';
                        initVal = "None";
                    end
                    if obj.propertyList(idx).dispType  ~= 'M'
                        widthInputField = obj.widthInputField;
                        labelText = obj.propertyList(idx).eltNames(startIdx + j);
                        obj.makeUiLabel(labelText, [obj.currXpos, obj.currYpos, widthLabel, obj.defEltHeight],...
                                       'center', [1 1 1], [0.1 0.1 0.1]);
                        obj.moveXposRight(widthLabel);
                    else
                        widthInputField = widthLabel;
                    end
                    child = obj.makeUiEditField(fieldType, initVal, [.95, .95, .95], [.1, .1, .1], ...
                                               [obj.currXpos, obj.currYpos, widthInputField, obj.defEltHeight]);
                    obj.propertyStateArray(idx).addEditChild(child);
                    obj.moveXposRight(widthInputField);
                end
                startIdx = startIdx + obj.propertyList(idx).dispDims(i);
                fillWidth = obj.figWidth - obj.borderWidth - obj.currXpos;
                if fillWidth > 0
                    fillLabelPos = [obj.currXpos, obj.currYpos, fillWidth, obj.defEltHeight];
                    obj.makeUiLabel("", fillLabelPos, 'center', [.96 .96 .96], [0.1 0.1 0.1]);
                end
                obj.moveYposDown();
                obj.resetXposLeft();
            end
            if obj.propertyStateArray(idx).property.processData
                width = obj.computeWidth(obj.processDataButtonTxt);
                obj.processDataButton = obj.makeUiButton(obj.processDataButtonTxt, [obj.currXpos, obj.currYpos, width, obj.defEltHeight],...
                                     [.96, .5, .5], [1, 1, 1], obj.propertyStateArray(idx).property.processDataFunc, {obj.propertyStateArray(idx).editChildren});
            end
        end

        function Ktx(obj, buf)
            count = 0;
            for i = 1:9:45
                txBuf = single.empty();
                txBuf(2:10) = buf(i:(i+8));
                txBuf(1) = single(count);
                txBuf = typecast(txBuf, 'uint8');
                obj.tvc.SetProperty(54, txBuf);
                count = count + 1;
                pause(0.5);
            end
        end

        function Krx(obj, propIdx)
            for j = 1:5
                obj.tvc.RequestProperty(54);
                pause(1);
                obj.tvc.RequestValue(54);
                pause(1);
                [id, sz, t, val] = obj.tvc.ReadValue();
                castType = obj.propertyStateArray(propIdx).property.type;
                dataBuf = typecast(uint8(val), castType);
                
                row = (double(dataBuf(1)) * 9) + 1;
                count = 2;
                for i = row:(row + 8)
                    obj.propertyStateArray(propIdx).editChildren(i).Value = double(dataBuf(count));
                    count = count + 1;
                end
            end    
        end

        function rx(obj, propIdx)
            if obj.propertyStateArray(propIdx).property.id == 54
                obj.Krx(propIdx);
                return;
            end
            obj.tvc.RequestProperty(obj.propertyStateArray(propIdx).property.id);
            pause(0.1);
            obj.tvc.RequestValue(obj.propertyStateArray(propIdx).property.id);
            [id, sz, t, val] = obj.tvc.ReadValue();
            castType = obj.propertyStateArray(propIdx).property.type;
            dataBuf = typecast(uint8(val), castType);
            for i = 1:obj.propertyStateArray(propIdx).property.numElts
                obj.propertyStateArray(propIdx).editChildren(i).Value = double(dataBuf(i));
            end
        end

        function tx(obj, propIdx)
            for i = 1:obj.propertyStateArray(propIdx).property.numElts
                txBuf_raw(i) = obj.propertyStateArray(propIdx).property.castFunc(obj.propertyStateArray(propIdx).editChildren(i).Value);
            end
            
            if obj.propertyStateArray(propIdx).property.numElts == 0
                txBuf = uint8(1);
            
            elseif obj.propertyStateArray(propIdx).property.id == 54
                obj.Ktx(txBuf_raw);
                return;
            else
                txBuf = typecast(txBuf_raw, 'uint8');
            end
            
            %disp(txBuf);
            
            
            obj.tvc.SetProperty(obj.propertyStateArray(propIdx).property.id, txBuf);
        end
    end
end