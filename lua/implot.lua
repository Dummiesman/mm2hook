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
  return ImPlot.BeginPlot(title_id, x_label, y_label, size, flags, x_flags, y_flags, y2_flags, y3_flags)
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
  ImPlot.PlotText(text, x, y, pix_offset, flags)
  flags = flags or 0
end

--Make global constants
ImPlotFlags_None                            = 0
ImPlotFlags_NoTitle                         = 1 << 0
ImPlotFlags_NoLegend                        = 1 << 1
ImPlotFlags_NoMouseText                     = 1 << 2
ImPlotFlags_NoInputs                        = 1 << 3
ImPlotFlags_NoMenus                         = 1 << 4
ImPlotFlags_NoBoxSelect                     = 1 << 5
ImPlotFlags_NoChild                         = 1 << 6
ImPlotFlags_NoFrame                         = 1 << 7
ImPlotFlags_Equal                           = 1 << 8
ImPlotFlags_Crosshairs                      = 1 << 9
ImPlotFlags_CanvasOnly                      = ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMouseText
                              
ImPlotAxisFlags_None                        = 0
ImPlotAxisFlags_NoLabel                     = 1 << 0
ImPlotAxisFlags_NoGridLines                 = 1 << 1
ImPlotAxisFlags_NoTickMarks                 = 1 << 2
ImPlotAxisFlags_NoTickLabels                = 1 << 3
ImPlotAxisFlags_NoInitialFit                = 1 << 4
ImPlotAxisFlags_NoMenus                     = 1 << 5
ImPlotAxisFlags_NoSideSwitch                = 1 << 6
ImPlotAxisFlags_NoHighlight                 = 1 << 7
ImPlotAxisFlags_Opposite                    = 1 << 8
ImPlotAxisFlags_Foreground                  = 1 << 9
ImPlotAxisFlags_Invert                      = 1 << 10
ImPlotAxisFlags_AutoFit                     = 1 << 11
ImPlotAxisFlags_RangeFit                    = 1 << 12
ImPlotAxisFlags_PanStretch                  = 1 << 13
ImPlotAxisFlags_LockMin                     = 1 << 14
ImPlotAxisFlags_LockMax                     = 1 << 15
ImPlotAxisFlags_Lock                        = ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax
ImPlotAxisFlags_NoDecorations               = ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels
ImPlotAxisFlags_AuxDefault                  = ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_Opposite

ImPlotTextFlags_None                        = 0
ImPlotTextFlags_Vertical                    = 1 << 10
    
return M