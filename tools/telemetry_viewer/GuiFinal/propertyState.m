classdef propertyState < handle
    properties
        property
        currState = 0;
        stateCanChange = 0;

        uiButtonChildren
        currentButtonChild = 1;
        uiEditChildren
        currentEditChild = 1;
    end
    methods
        function p = propertyState(property)
            p.property = property;
            p.uiButtonChildren = zeros(1,3);
            if property.dispDims(1) == 0
                p.uiEditChildren = [];
            else
                p.uiEditChildren = zeros(1, p.property.numElts);
            end
        end
        
        function p = addButtonChild(p,num)
            p.uiButtonChildren(p.currentButtonChild) = num;
            p.currentButtonChild = p.currentButtonChild + 1;
        end

        function p = addEditChild(p,num)
            p.uiEditChildren(p.currentEditChild) = num;
            p.currentEditChild = p.currentEditChild + 1;
        end

        function p = initChildren(p, childArray)
            for i=1:size(p.uiButtonChildren,2)
                p.uiButtonChildren(i)=(childArray(p.uiButtonChildren(i)));
            end
            for i=1:size(p.uiEditChildren,2)
                p.uiEditChildren(i)=(childArray(p.uiEditChildren(i)));
            end
        end
    end
end
