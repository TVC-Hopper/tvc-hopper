function val = getProperty(tvc, id, type)
    tvc.RequestProperty(id);
    pause(0.5);
    tvc.RequestValue(id);
    [~, ~, ~, raw_val] = tvc.ReadValue();
    
    val = typecast(raw_val, type);
end

