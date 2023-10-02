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
M.BeginPlot = function(title, size, flags)  
  size = size or ImVec2(-1, 0)
  flags = flags or 0
  return ImPlot.BeginPlot(title, size, flags)
end

M.EndPlot = function()
  ImPlot.EndPlot()
end

M.PlotLine = function(label, xValues, yValues, offset)
  offset = offset or 0
  ImPlot.PlotLine(label, xValues, yValues, offset)
end

M.PlotScatter = function(label, xValues, yValues, offset)
  offset = offset or 0
  ImPlot.PlotScatter(label, xValues, yValues, offset)
end

M.PlotBars = function(label, xValues, yValues, width, offset)
  offset = offset or 0
  width = width or 1
  ImPlot.PlotBars(label, xValues, yValues, width, offset)
end

M.PlotText = function(text, x, y, pix_offset, flags)
  pix_offset = pix_offset or ImVec2(0, 0)
  flags = flags or 0
  ImPlot.PlotText(text, x, y, pix_offset, flags)
end

M.SetupAxis = function(axis, label, flags)
  label = nullableString(label)
  flags = flags or 0
  ImPlot.SetupAxis(axis, label, flags)
end

M.SetupAxes = function(x_label, y_label, x_flags, y_flags)
  x_label = nullableString(x_label)
  y_label = nullableString(y_label)
  x_flags = x_flags or 0
  y_flags = y_flags or 0
  ImPlot.SetupAxes(x_label, y_label, x_flags, y_flags)
end

M.SetupLegend = function(location, flags)
  ImPlot.SetupLegend(location, flags)
end

M.SetupAxisTicks = function(axis, min, max, n_ticks, labels, show_default)
  show_default = boolDefault(show_default, false)
  ImPlot.SetupAxisTicks(axis, min, max, n_ticks, labels, show_default)
end

M.SetupFinish = function()
  ImPlot.SetupFinish()
end

M.BeginAlignedPlots = function(group_id, vertical)
  vertical = boolDefault(vertical, true)
  return ImPlot.BeginAlignedPlots(group_id, vertical)
end

M.EndAlignedPlots = function()
  ImPlot.EndAlignedPlots()
end

--Make global constants
ImAxis_X1 = 0
ImAxis_X2 = 1
ImAxis_X3 = 2
ImAxis_Y1 = 3
ImAxis_Y2 = 4
ImAxis_Y3 = 5
    
ImPlotFlags_None          = 0
ImPlotFlags_NoTitle       = 1 << 0
ImPlotFlags_NoLegend      = 1 << 1
ImPlotFlags_NoMouseText   = 1 << 2
ImPlotFlags_NoInputs      = 1 << 3
ImPlotFlags_NoMenus       = 1 << 4
ImPlotFlags_NoBoxSelect   = 1 << 5
ImPlotFlags_NoChild       = 1 << 6
ImPlotFlags_NoFrame       = 1 << 7
ImPlotFlags_Equal         = 1 << 8
ImPlotFlags_Crosshairs    = 1 << 9
ImPlotFlags_CanvasOnly    = ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMouseText

ImPlotAxisFlags_None          = 0
ImPlotAxisFlags_NoLabel       = 1 << 0
ImPlotAxisFlags_NoGridLines   = 1 << 1
ImPlotAxisFlags_NoTickMarks   = 1 << 2
ImPlotAxisFlags_NoTickLabels  = 1 << 3
ImPlotAxisFlags_NoInitialFit  = 1 << 4
ImPlotAxisFlags_NoMenus       = 1 << 5
ImPlotAxisFlags_NoSideSwitch  = 1 << 6
ImPlotAxisFlags_NoHighlight   = 1 << 7
ImPlotAxisFlags_Opposite      = 1 << 8
ImPlotAxisFlags_Foreground    = 1 << 9
ImPlotAxisFlags_Invert        = 1 << 10
ImPlotAxisFlags_AutoFit       = 1 << 11
ImPlotAxisFlags_RangeFit      = 1 << 12
ImPlotAxisFlags_PanStretch    = 1 << 13
ImPlotAxisFlags_LockMin       = 1 << 14
ImPlotAxisFlags_LockMax       = 1 << 15
ImPlotAxisFlags_Lock          = ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax
ImPlotAxisFlags_NoDecorations = ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels
ImPlotAxisFlags_AuxDefault    = ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_Opposite

ImPlotSubplotFlags_None        = 0
ImPlotSubplotFlags_NoTitle     = 1 << 0
ImPlotSubplotFlags_NoLegend    = 1 << 1
ImPlotSubplotFlags_NoMenus     = 1 << 2
ImPlotSubplotFlags_NoResize    = 1 << 3
ImPlotSubplotFlags_NoAlign     = 1 << 4
ImPlotSubplotFlags_ShareItems  = 1 << 5
ImPlotSubplotFlags_LinkRows    = 1 << 6
ImPlotSubplotFlags_LinkCols    = 1 << 7
ImPlotSubplotFlags_LinkAllX    = 1 << 8
ImPlotSubplotFlags_LinkAllY    = 1 << 9
ImPlotSubplotFlags_ColMajor    = 1 << 10

ImPlotLegendFlags_None            = 0
ImPlotLegendFlags_NoButtons       = 1 << 0
ImPlotLegendFlags_NoHighlightItem = 1 << 1
ImPlotLegendFlags_NoHighlightAxis = 1 << 2
ImPlotLegendFlags_NoMenus         = 1 << 3
ImPlotLegendFlags_Outside         = 1 << 4
ImPlotLegendFlags_Horizontal      = 1 << 5
ImPlotLegendFlags_Sort            = 1 << 6

ImPlotMouseTextFlags_None        = 0
ImPlotMouseTextFlags_NoAuxAxes   = 1 << 0
ImPlotMouseTextFlags_NoFormat    = 1 << 1
ImPlotMouseTextFlags_ShowAlways  = 1 << 2

ImPlotDragToolFlags_None      = 0    
ImPlotDragToolFlags_NoCursors = 1 << 0
ImPlotDragToolFlags_NoFit     = 1 << 1
ImPlotDragToolFlags_NoInputs  = 1 << 2
ImPlotDragToolFlags_Delayed   = 1 << 3

ImPlotColormapScaleFlags_None     = 0
ImPlotColormapScaleFlags_NoLabel  = 1 << 0
ImPlotColormapScaleFlags_Opposite = 1 << 1
ImPlotColormapScaleFlags_Invert   = 1 << 2

ImPlotItemFlags_None     = 0
ImPlotItemFlags_NoLegend = 1 << 0
ImPlotItemFlags_NoFit    = 1 << 1

ImPlotLineFlags_None        = 0
ImPlotLineFlags_Segments    = 1 << 10
ImPlotLineFlags_Loop        = 1 << 11
ImPlotLineFlags_SkipNaN     = 1 << 12
ImPlotLineFlags_NoClip      = 1 << 13
ImPlotLineFlags_Shaded      = 1 << 14

ImPlotLocation_Center    = 0                                      
ImPlotLocation_North     = 1 << 0                                   
ImPlotLocation_South     = 1 << 1                                   
ImPlotLocation_West      = 1 << 2                                   
ImPlotLocation_East      = 1 << 3                                   
ImPlotLocation_NorthWest = ImPlotLocation_North | ImPlotLocation_West
ImPlotLocation_NorthEast = ImPlotLocation_North | ImPlotLocation_East
ImPlotLocation_SouthWest = ImPlotLocation_South | ImPlotLocation_West
ImPlotLocation_SouthEast = ImPlotLocation_South | ImPlotLocation_East
    
return M