local M = {}

--IMNODEEDITOR Layer that provides default parameters etc. For your sanity.
--Forwarded functions are on a single line. (with some exceptions like push/pop, and multiple possible default vals) 
--Other functions have multi line bodies.

--Internal Constants

--Helpers
local function boolDefault(val, def)
  if val == nil then return def end
  if type(val) ~= 'boolean' then return def end
  return val
end

local function nullableString(str)
  return str or ""
end

--IMNODEEDITOR
M.DestroyEditor =               function(ctx)         ImNodeEditor.DestroyEditor(ctx)                           end
M.SetCurrentEditor =            function(ctx)         ImNodeEditor.SetCurrentEditor(ctx)                        end
M.GetCurrentEditor =            function()            return ImNodeEditor.GetCurrentEditor()                    end
M.GetCurrentZoom =              function()            return ImNodeEditor.GetCurrentZoom()                      end
                
M.AcceptNewItem =               function()            return ImNodeEditor.AcceptNewItem()                       end
M.AcceptDeletedItem =           function()            return ImNodeEditor.AcceptDeletedItem()                   end
M.AcceptCopy =                  function()            return ImNodeEditor.AcceptCopy()                          end
M.AcceptCut =                   function()            return ImNodeEditor.AcceptCut()                           end
M.AcceptDuplicate =             function()            return ImNodeEditor.AcceptDuplicate()                     end
M.AcceptPaste =                 function()            return ImNodeEditor.AcceptPaste()                         end
M.AcceptCreateNode =            function()            return ImNodeEditor.AcceptCreateNode()                    end
                
M.PinPivotRect =                function(a, b)        ImNodeEditor.PinPivotRect(a, b)                           end
M.PinPivotAlignment =           function(alignment)   ImNodeEditor.PinPivotAlignment(alignment)                 end
M.PinPivotScale =               function(scale)       ImNodeEditor.PinPivotScale(scale)                         end
M.PinPivotSize =                function(size)        ImNodeEditor.PinPivotSize(size)                           end
                
M.CenterNodeOnScreen =          function(id)          ImNodeEditor.CenterNodeOnScreen(id)                       end
M.CanvasToScreen =              function(pos)         return ImNodeEditor.CanvasToScreen(pos)                   end
                
M.ClearSelection =              function()            ImNodeEditor.ClearSelection()                             end
  
M.SelectLink =                  function(id, append)  ImNodeEditor.SelectLink(id, boolDefault(append, false))   end
M.SelectNode =                  function(id, append)  ImNodeEditor.SelectNode(id, boolDefault(append, false))   end
M.DeselectLink =                function(id, append)  ImNodeEditor.DeselectLink(id, boolDefault(append, false)) end
M.DeselectNode =                function(id, append)  ImNodeEditor.DeselectNode(id, boolDefault(append, false)) end
M.IsLinkSelected =              function(id)          return ImNodeEditor.IsLinkSelected(id)                    end
M.IsNodeSelected =              function(id)          return ImNodeEditor.IsNodeSelected(id)                    end

M.PinHasAnyLinks =              function(id)          return ImNodeEditor.PinHasAnyLinks(id)                    end
M.NodeHasAnyLinks =             function(id)          return ImNodeEditor.NodeHasAnyLinks(id)                   end
    
M.SetNodePosition =             function(id, pos)     ImNodeEditor.SetNodePosition(id, pos)                     end
M.SetNodeZPosition =            function(id, z)       ImNodeEditor.SetNodeZPosition(id, z)                      end
M.GetNodePosition =             function(id)          return ImNodeEditor.GetNodePosition(id)                   end
M.GetNodeSize =                 function(id)          return ImNodeEditor.GetNodeSize(id)                       end
M.GetNodeZPosition =            function(id)          return ImNodeEditor.GetNodeZPosition(id)                  end

M.Suspend =                     function()            ImNodeEditor.Suspend()                                    end
M.Resume =                      function()            ImNodeEditor.Resume()                                     end
M.IsSuspended =                 function()            return ImNodeEditor.IsSuspended()                         end

--
M.CreateEditor = function(config)
  if not config then
    config = ImNodeEditorConfig()
    config.SettingsFile = "imgui_nodes.json"
  end
  return ImNodeEditor.CreateEditor(config)
end

M.Icon = function(size, type, filled, color, innerColor)
  color = color or ImVec4(1.0, 1.0, 1.0, 1.0)
  innerColor = innerColor or ImVec4(0.0, 0.0, 0.0, 0.0)
  ImNodeEditor.Icon(size, type, filled, color, innerColor)
end

M.QueryNewLink = function()
  return ImNodeEditor.QueryNewLink()
end

M.QueryNewNode = function()
  return ImNodeEditor.QueryNewNode()
end

M.QueryDeletedLink = function()
  return ImNodeEditor.QueryDeletedLink()
end

M.Link = function(id, pin1id, pin2id, color, thickness)
  color = color or ImVec4(1.0, 1.0, 1.0, 1.0)
  thickness = thickness or 1.0
  return ImNodeEditor.Link(id, pin1id, pin2id, color, thickness)
end

M.Flow = function(id, direction)
  ImNodeEditor.Flow(id, direction)
end

M.Begin = function(id, size)
  size = size or ImVec2(0.0, 0.0)
  ImNodeEditor.Begin(id, size)
end

M.BeginCreate = function(color, thickness)
  color = color or ImVec4(1.0, 1.0, 1.0, 1.0)
  thickness = thickness or 1.0
  return ImNodeEditor.BeginCreate(color, thickness)
end

M.BeginDelete = function()
  ImNodeEditor.BeginDelete()
end

M.BeginShortcut = function()
  ImNodeEditor.BeginShortcut()
end

M.BeginNode = function(id) 
  ImNodeEditor.BeginNode(id)
end

M.BeginPin = function(id, kind)
  ImNodeEditor.BeginPin(id, kind)
end

M.EndNode = function()
  ImNodeEditor.EndNode()
end

M.EndPin = function()
  ImNodeEditor.EndPin()
end

M.EndShortcut = function()
  ImNodeEditor.EndShortcut()
end

M.EndDelete = function()
  ImNodeEditor.EndDelete()
end

M.EndCreate = function()
  ImNodeEditor.EndCreate()
end

M.End = function()
  ImNodeEditor.End()
end

M.PushStyleVar = function(id, val)  
  --check what variant of the function to use
  if type(val) == 'number' then
    ImNodeEditor.PushStyleVarFloat(id, val)
  else
    local internalType = val.___type
    if internalType == 'class<ImVec4>' then
      ImNodeEditor.PushStyleVarVec4(id, val)
    else
      ImNodeEditor.PushStyleVarVec2(id, val)
    end
  end
end

M.PushStyleColor = function(id, val)
  ImNodeEditor.PushStyleColor(id, val)
end

M.PopStyleVar = function(count)
  count = count or 1
  ImNodeEditor.PopStyleVar(count)
end

M.PopStyleColor = function(count)
  count = count or 1
  ImNodeEditor.PopStyleColor(count)
end

--Make global constants
ImNodeEditorPinKind_Input  = 0
ImNodeEditorPinKind_Output = 1

ImNodeEditorIcon_Flow        = 0
ImNodeEditorIcon_Circle      = 1
ImNodeEditorIcon_Square      = 2
ImNodeEditorIcon_Grid        = 3
ImNodeEditorIcon_RoundSquare = 4
ImNodeEditorIcon_Diamond     = 5

ImNodeEditorFlowForward  = 0
ImNodeEditorFlowBackward = 1

ImNodeEditorStyleVar_NodePadding              = 0
ImNodeEditorStyleVar_NodeRounding             = 1
ImNodeEditorStyleVar_NodeBorderWidth          = 2
ImNodeEditorStyleVar_HoveredNodeBorderWidth   = 3
ImNodeEditorStyleVar_SelectedNodeBorderWidth  = 4
ImNodeEditorStyleVar_PinRounding              = 5
ImNodeEditorStyleVar_PinBorderWidth           = 6
ImNodeEditorStyleVar_LinkStrength             = 7
ImNodeEditorStyleVar_SourceDirection          = 8
ImNodeEditorStyleVar_TargetDirection          = 9
ImNodeEditorStyleVar_ScrollDuration           = 10
ImNodeEditorStyleVar_FlowMarkerDistance       = 11
ImNodeEditorStyleVar_FlowSpeed                = 12
ImNodeEditorStyleVar_FlowDuration             = 13
ImNodeEditorStyleVar_PivotAlignment           = 14
ImNodeEditorStyleVar_PivotSize                = 15
ImNodeEditorStyleVar_PivotScale               = 16
ImNodeEditorStyleVar_PinCorners               = 17
ImNodeEditorStyleVar_PinRadius                = 18
ImNodeEditorStyleVar_PinArrowSize             = 19
ImNodeEditorStyleVar_PinArrowWidth            = 20
ImNodeEditorStyleVar_GroupRounding            = 21
ImNodeEditorStyleVar_GroupBorderWidth         = 22
ImNodeEditorStyleVar_HighlightConnectedLinks  = 23
ImNodeEditorStyleVar_SnapLinkToPinDir         = 24
ImNodeEditorStyleVar_HoveredNodeBorderOffset  = 25
ImNodeEditorStyleVar_SelectedNodeBorderOffset = 26

ImNodeEditorStyleColor_Bg                     = 0
ImNodeEditorStyleColor_Grid                   = 1
ImNodeEditorStyleColor_NodeBg                 = 2
ImNodeEditorStyleColor_NodeBorder             = 3
ImNodeEditorStyleColor_HovNodeBorder          = 4
ImNodeEditorStyleColor_SelNodeBorder          = 5
ImNodeEditorStyleColor_NodeSelRect            = 6
ImNodeEditorStyleColor_NodeSelRectBorder      = 7
ImNodeEditorStyleColor_HovLinkBorder          = 8
ImNodeEditorStyleColor_SelLinkBorder          = 9
ImNodeEditorStyleColor_HighlightLinkBorder    = 10
ImNodeEditorStyleColor_LinkSelRect            = 11
ImNodeEditorStyleColor_LinkSelRectBorder      = 12
ImNodeEditorStyleColor_PinRect                = 13
ImNodeEditorStyleColor_PinRectBorder          = 14
ImNodeEditorStyleColor_Flow                   = 15
ImNodeEditorStyleColor_FlowMarker             = 16
ImNodeEditorStyleColor_GroupBg                = 17
ImNodeEditorStyleColor_GroupBorder            = 18
    
return M