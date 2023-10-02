local M = {}

--IMGUIZMO Layer that provides default parameters etc. For your sanity.
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

--IMGUIZMO
M.SetID =                 function(id)     ImGuizmo.SetID(id)                    end
M.SetGizmoSizeClipSpace = function(value)  ImGuizmo.SetGizmoSizeClipSpace(value) end
M.Enable =                function(enable) ImGuizmo.Enable(enable)               end
M.AllowAxisFlip =         function(value)  ImGuizmo.AllowAxisFlip(value)         end
M.SetAxisLimit =          function(value)  ImGuizmo.SetAxisLimit(value)          end
M.SetPlaneLimit =         function(value)  ImGuizmo.SetPlaneLimit(value)         end
M.SetOrthographic =       function(value)  ImGuizmo.SetOrthographic(value)       end
M.IsUsing =               function()       return ImGuizmo.IsUsing()             end
M.IsUsingAny =            function()       return ImGuizmo.IsUsingAny()          end
M.GetStyle =              function()       return ImGuizmo.GetStyle()            end

M.IsOver = function(op_mask)
  op_mask = op_mask or 14463
  return ImGuizmo.IsOver(op_mask)
end

M.Manipulate = function(view, proj, op, mode, pmtx)
  return ImGuizmo.Manipulate(view, proj, op, mode, pmtx)
end

M.DrawGrid = function(view, proj, mtx, gridSize)
  ImGuizmo.DrawGrid(view, proj, mtx, gridSize)
end

M.SetRect = function(x, y, w, h)
  if type(x) == 'number' then
    ImGuizmo.SetRect(x, y, w, h)
  else
    ImGuizmo.SetRect(x.x, x.y, x.z, x.w)
  end
end


--Make global constants
OP_TRANSLATE_X = 1 << 0
OP_TRANSLATE_Y = 1 << 1
OP_TRANSLATE_Z = 1 << 2
OP_ROTATE_X = 1 << 3
OP_ROTATE_Y = 1 << 4
OP_ROTATE_Z = 1 << 5
OP_ROTATE_SCREEN = 1 << 6
OP_SCALE_X = 1 << 7
OP_SCALE_Y = 1 << 8
OP_SCALE_Z = 1 << 9
OP_BOUNDS = 1 << 10
OP_SCALE_XU = 1 << 11
OP_SCALE_YU = 1 << 12
OP_SCALE_ZU = 1 << 13

OP_TRANSLATE = OP_TRANSLATE_X | OP_TRANSLATE_Y | OP_TRANSLATE_Z
OP_ROTATE = OP_ROTATE_X | OP_ROTATE_Y | OP_ROTATE_Z | OP_ROTATE_SCREEN
OP_SCALE = OP_SCALE_X | OP_SCALE_Y | OP_SCALE_Z
OP_SCALEU = OP_SCALE_XU | OP_SCALE_YU | OP_SCALE_ZU
OP_UNIVERSAL = OP_TRANSLATE | OP_ROTATE | OP_SCALEU

MODE_LOCAL = 0
MODE_WORLD = 1
    
return M