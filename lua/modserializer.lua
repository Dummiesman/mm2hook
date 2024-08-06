local M = {}

---@type mmPlayerDirectory
local playerDirectory

local function ensurePlayerDirectoryExists()
    if playerDirectory then return end
    playerDirectory = mmPlayerDirectory()
    playerDirectory:SetIOPath(lfs.currentdir() .. "\\players")    
    playerDirectory:Load("players")
end

local function dirname(path)
    local dir = path:gsub("[^\\]+\\*$", "")
    if dir == "" then return path end
    return dir
end

local function getDataPath(modName, serializationMode)
    if serializationMode == "file" then
        return string.format("%s\\players\\scriptdata\\%s", lfs.currentdir(), modName)
    else
        ensurePlayerDirectoryExists()
        local playerIndex = playerDirectory:FindPlayer(playerDirectory:GetLastPlayer())
        if playerIndex >= 0 then
            local playerFileName = playerDirectory:GetFileNameByIndex(playerIndex)
            return string.format("%s\\players\\%s\\scriptdata\\%s", lfs.currentdir(), playerFileName, modName)
        else
            return nil
        end
    end
end

local function createDirChain(path)
    local attrib = lfs.attributes(path)
    if attrib and attrib.mode == "directory" then
         return
    end

    local chain = {}
    repeat
        path = dirname(path)
        table.insert(chain, path)
    until lfs.attributes(path, "mode") == "directory"
    for i=#chain-1, 1, -1 do
       lfs.mkdir(chain[i])
    end
end

local function serializeMod(modTable)
    -- check if mod has serialization methods
    if type(modTable.serialize) ~= 'function' then return end

    -- serialize
    local mode = modTable.info.serializationMode or "temporary"
    local serializedData = modTable.serialize()
    if mode == "temporary" then
        PersistentDataStore.Store(modTable._modInternalName, json.encode(serializedData))
    elseif mode == "file" or mode == "playerfile" then
        local path = getDataPath(modTable._modInternalName, mode)
        if path == nil then 
            Errorf("Failed to get serialization path for %s", modTable._modInternalName)
            return
        end

        createDirChain(path)
        local stream = Stream.CreateWithMethods(path, Stream.StandardFileMethods)
        if stream then
            stream:Write(json.encode(serializedData))
            stream:Close()
        else
            Errorf("serializeMod: Unable to open stream '%s'", path)
        end
    else
        Errorf("serializeMod: Unknown serialization mode '%s'", mode)
    end
end

local function deserializeMod(modTable)
    -- check if mod has serialization methods
    if type(modTable.deserialize) ~= 'function' then
         return 
    end

    -- deserialize
    local mode = modTable.info.serializationMode or "temporary"
    if mode == "temporary" then
        if PersistentDataStore.Contains(modTable._modInternalName) then
            local serializedData = PersistentDataStore.Retrieve(modTable._modInternalName)
            modTable.deserialize(json.decode(serializedData))
            PersistentDataStore.Delete(modTable._modInternalName)
        end
    elseif mode == "file" or mode == "playerfile" then
        local path = getDataPath(modTable._modInternalName, mode)
        if path == nil then 
            Errorf("Failed to get serialization path for %s", modTable._modInternalName)
            return
        end

        local stream = Stream.OpenWithMethods(path, Stream.StandardFileMethods, true)
        if stream then
            local serializedData = stream:ReadAll()
            stream:Close()
            modTable.deserialize(json.decode(serializedData))
        end
    else
        Errorf("deserializeMod: Unknown serialization mode '%s'", mode)
    end
end

M.serialize = serializeMod
M.deserialize = deserializeMod
return M