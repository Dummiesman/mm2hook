function ParseCSV(text, separator, trimValues, allowEmptyValues, callback)
  local groups = {}; local cgroup = {}; local cvalue = ""
  
  separator = separator or ","
  if allowEmptyValues == nil then allowEmptyValues = false end
  if trimValues == nil then trimValues = true end
  
  local function newValue()
    if #cvalue ~= 0 or (#cvalue == 0 and allowEmptyValues) then
      if trimValues then cvalue = string.gsub(cvalue, "^%s*(.-)%s*$", "%1") end
      table.insert(cgroup, cvalue)
    end
    cvalue = ""
  end
  
  local function newLine()
    if #cgroup > 0 then 
      table.insert(groups, cgroup) 
      if callback then callback(cgroup) end
    end
    cgroup = {}
  end
  
  for i=1, #text do
    local c = text:sub(i,i)
    if c == "\n" then
      newValue()
      newLine()
    else
      if c == separator then
        newValue()
      elseif c ~= "\r" then
        cvalue = cvalue .. c
      end
    end
  end
  newValue()
  newLine()
  return groups
end