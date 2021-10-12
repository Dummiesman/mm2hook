local M = {}

--IMPLOT Layer that provides default parameters etc. For your sanity.
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

--IMPLOT

--
M.BeginPlot = function(title_id, x_label, y_label, size, flags, x_flags, y_flags, y2_flags, y3_flags)  
  x_label = nullableString(x_label)
  y_label = nullableString(y_label)
  flags = flags or 0
  x_flags = x_flags or 0
  y_flags = y_flags or 0
  y2_flags = y2_flags or 0
  y3_flags = y3_flags or 0
  size = size or ImVec2(-1, 0)
  return implot.BeginPlot(title_id, x_label, y_label, size, flags, x_flags, y_flags, y2_flags, y3_flags)
end

M.EndPlot = function()
  implot.EndPlot()
end

M.PlotLine = function(label, xValues, yValues, offset)
  offset = offset or 0
  implot.PlotLine(label, xValues, yValues, offset)
end

M.PlotScatter = function(label, xValues, yValues, offset)
  offset = offset or 0
  implot.PlotScatter(label, xValues, yValues, offset)
end

M.PlotBars = function(label, xValues, yValues, width, offset)
  offset = offset or 0
  width = width or 1
  implot.PlotBars(label, xValues, yValues, width, offset)
end

M.PlotText = function(text, x, y, vertical, pix_offset)
  vertical = boolDefault(vertical, false)
  pix_offset = pix_offset or ImVec2(0, 0)
  implot.PlotText(text, x, y, vertical, pix_offset)
end

--Make global constants
ImPlotFlags_None          = 0       -- default
ImPlotFlags_NoLegend      = 1 << 0  -- the top-left legend will not be displayed
ImPlotFlags_NoMenus       = 1 << 1  -- the user will not be able to open context menus with double-right click
ImPlotFlags_NoBoxSelect   = 1 << 2  -- the user will not be able to box-select with right-mouse
ImPlotFlags_NoMousePos    = 1 << 3  -- the mouse position, in plot coordinates, will not be displayed inside of the plot
ImPlotFlags_NoHighlight   = 1 << 4  -- plot items will not be highlighted when their legend entry is hovered
ImPlotFlags_NoChild       = 1 << 5  -- a child window region will not be used to capture mouse scroll (can boost performance for single ImGui window applications)
ImPlotFlags_YAxis2        = 1 << 6  -- enable a 2nd y-axis on the right side
ImPlotFlags_YAxis3        = 1 << 7  -- enable a 3rd y-axis on the right side
ImPlotFlags_Query         = 1 << 8  -- the user will be able to draw query rects with middle-mouse
ImPlotFlags_Crosshairs    = 1 << 9  -- the default mouse cursor will be replaced with a crosshair when hovered
ImPlotFlags_AntiAliased   = 1 << 10 -- plot lines will be software anti-aliased (not recommended for density plots, prefer MSAA)
ImPlotFlags_CanvasOnly    = ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMousePos

ImPlotAxisFlags_None          = 0      -- default
ImPlotAxisFlags_NoGridLines   = 1 << 0 -- no grid lines will be displayed
ImPlotAxisFlags_NoTickMarks   = 1 << 1 -- no tick marks will be displayed
ImPlotAxisFlags_NoTickLabels  = 1 << 2 -- no text labels will be displayed
ImPlotAxisFlags_LogScale      = 1 << 3 -- a logartithmic (base 10) axis scale will be used (mutually exclusive with ImPlotAxisFlags_Time)
ImPlotAxisFlags_Time          = 1 << 4 -- axis will display date/time formatted labels (mutually exclusive with ImPlotAxisFlags_LogScale)
ImPlotAxisFlags_Invert        = 1 << 5 -- the axis will be inverted
ImPlotAxisFlags_LockMin       = 1 << 6 -- the axis minimum value will be locked when panning/zooming
ImPlotAxisFlags_LockMax       = 1 << 7 -- the axis maximum value will be locked when panning/zooming
ImPlotAxisFlags_Lock          = ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax
ImPlotAxisFlags_NoDecorations = ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels
    
return M