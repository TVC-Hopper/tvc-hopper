classdef propertyState < handle
    properties
        property
        currState = 0;
        stateCanChange = 0;

        buttonChildren
        editChildren
    end
    methods
        function p = propertyState(property)
            p.property = property;
            p.initChildArrays();
        end

        function initChildArrays(p)
            fig = uifigure('visible', 'off');
            p.buttonChildren = uibutton(fig).empty(1, 0);
            p.editChildren = uieditfield(fig).empty(1, 0);
            delete(fig);
        end
        
        function p = addButtonChild(p, child)
            p.buttonChildren(end + 1) = child;
        end

        function p = addEditChild(p, child)
            p.editChildren(end + 1) = child;
        end
    end
end
