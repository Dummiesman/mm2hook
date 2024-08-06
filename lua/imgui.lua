local M = {}

--IMGUI Layer that provides default parameters etc. For your sanity.
--Forwarded functions are on a single line. (with some exceptions like push/pop, and multiple possible default vals) 
--Other functions have multi line bodies.

--Internal Constants
local INT_SLIDER_MIN = -2147483648
local INT_SLIDER_MAX = 2147483647
local FLOAT_SLIDER_MIN = -3.40282347e+38 / 2
local FLOAT_SLIDER_MAX = 3.40282347e+38 / 2
local ZERO_VEC2 = ImVec2(0, 0)
local DEFAULT_ANGLE_FORMAT = "%.0f deg"
local DEFAULT_FLOAT_FORMAT = "%.3f"
local DEFAULT_INT_FORMAT = "%d"

--Helpers
local function boolDefault(val, def)
  if val == nil then return def end
  if type(val) ~= 'boolean' then return def end
  return val
end

local function nullableString(str)
  return str or ""
end

M.BeginRadioButtonGroup = function(sid, startId)
  M.radioButtonGroupOpen = true
  M.radioButtonGroupSid = sid
  M.radioButtonGroupId = startId or 1
end

M.EndRadioButtonGroup = function()
  M.radioButtonGroupOpen = false
  return M.radioButtonGroupSid
end

--IMGUI
M.BeginTooltip =                      function()               ImGui.BeginTooltip()                                       end
M.EndTooltip =                        function()               ImGui.EndTooltip()                                         end
                                                                                                                          
M.BeginMenu =                         function(mNam, enable)   return ImGui.BeginMenu(mNam, boolDefault(enable, true))    end
M.EndMenu =                           function()               ImGui.EndMenu()                                            end
M.BeginMenuBar =                      function()               return ImGui.BeginMenuBar()                                end
M.EndMenuBar =                        function()               ImGui.EndMenuBar()                                         end
M.BeginMainMenuBar =                  function()               return ImGui.BeginMainMenuBar()                            end
M.EndMainMenuBar =                    function()               ImGui.EndMainMenuBar()                                     end
                                                                                                                          
M.Dummy =                             function(sz)             ImGui.Dummy(sz or ZERO_VEC2)                               end        
                                                                                                                          
M.BeginDisabled =                     function(disabled)       ImGui.BeginDisabled(boolDefault(disabled, true))           end
M.EndDisabled =                       function()               ImGui.EndDisabled()                                        end
                                                                                                                          
M.HelpMarker =                        function(text)           ImGui.HelpMarker(text)                                     end
M.Separator =                         function()               ImGui.Separator()                                          end
M.SeparatorText =                     function(text)               ImGui.SeparatorText(text)                              end
                                                                                                                          
M.AlignTextToFramePadding =           function()               ImGui.AlignTextToFramePadding()                            end
M.GetTextLineHeight =                 function()               return ImGui.GetTextLineHeight()                           end
M.CalcItemWidth =                     function()               return ImGui.CalcItemWidth()                               end
M.GetFrameCount =                     function()               return ImGui.GetFrameCount()                               end
M.GetFrameHeight =                    function()               return ImGui.GetFrameHeight()                              end
M.GetFrameHeightWithSpacing =         function()               return ImGui.GetFrameHeightWithSpacing()                   end
M.Text =                              function(text)           ImGui.Text(text)                                           end
M.TextWrapped =                       function(text)           ImGui.TextWrapped(text)                                    end
M.TextColored =                       function(color, text)    ImGui.TextColored(color, text)                             end
M.LabelText =                         function(label, text)    ImGui.LabelText(label, text)                               end
M.BulletText =                        function(text)           ImGui.BulletText(text)                                     end
                                                                                                                          
M.GetFont =                           function()               return ImGui.GetFont()                                     end       
M.GetFontSize =                       function()               return ImGui.GetFontSize()                                 end
M.SetWindowFontScale =                function(scale)          ImGui.SetWindowFontScale(scale)                            end       
M.PushFont =                          function(font)           ImGui.PushFont(font)                                       end       
M.PopFont =                           function()               ImGui.PopFont()                                            end       
                                                                                                                          
M.GetColumnIndex =                    function()               return ImGui.GetColumnIndex()                              end
M.GetColumnOffset =                   function(index)          return ImGui.GetColumnOffset(index or -1)                  end
M.GetColumnsCount =                   function()               return ImGui.GetColumnsCount()                             end
M.GetColumnWidth =                    function(index)          return ImGui.GetColumnWidth(index or -1)                   end
M.NextColumn =                        function()               ImGui.NextColumn()                                         end
M.SetColumnOffset =                   function(index, offset)  ImGui.SetColumnOffset(index, offset)                       end
M.SetColumnWidth =                    function(index, width)   ImGui.SetColumnWidth(index, width)                         end
                                                                                                                          
M.GetMousePos =                       function()               return ImGui.GetMousePos()                                 end        
M.GetMousePosOnOpeningCurrentPopup =  function()               return ImGui.GetMousePosOnOpeningCurrentPopup()            end        
M.IsMouseDown =                       function(btn)            return ImGui.IsMouseDown(btn)                              end
M.IsMouseClicked =                    function(btn,rp)         return ImGui.IsMouseClicked(btn, boolDefault(rp,false))    end
M.IsMouseReleased =                   function(btn)            return ImGui.IsMouseReleased(btn)                          end
M.IsAnyMouseDown =                    function()               return ImGui.IsAnyMouseDown()                              end
M.IsMouseDragging =                   function(btn, lockThrsh) return ImGui.IsMouseDragging(btn. lockThrsh or -1.0)       end
M.GetMouseDragDelta =                 function(btn, lockThrsh) return ImGui.GetMouseDragDelta(btn, lockThrsh or -1.0)     end

M.IsKeyPressed =                      function(key, _repeat)   return ImGui.IsKeyPressed(key, boolDefault(_repeat, true)) end
M.IsKeyReleased =                     function(key)            return ImGui.IsKeyReleased(key)                            end
M.SetKeyboardFocusHere =              function(offset)         ImGui.SetKeyboardFocusHere(offset or 0)                    end

M.IsWindowFocused =                   function(focusFlags)     return ImGui.IsWindowFocused(focusFlags or 0)              end
M.IsWindowHovered =                   function(hoverFlags)     return ImGui.IsWindowHovered(hoverFlags or 0)              end
M.IsItemActive =                      function()               return ImGui.IsItemActive()                                end
M.IsItemFocused =                     function()               return ImGui.IsItemFocused()                               end
M.IsItemHovered =                     function(hoverFlags)     return ImGui.IsItemHovered(hoverFlags or 0)                end
M.IsAnyItemActive =                   function()               return ImGui.IsAnyItemActive()                             end
M.IsAnyItemFocused =                  function()               return ImGui.IsAnyItemFocused()                            end
M.IsAnyItemHovered =                  function()               return ImGui.IsAnyItemHovered()                            end
M.IsItemActivated =                   function()               return ImGui.IsItemActivated()                             end
M.IsItemVisible =                     function()               return ImGui.IsItemVisible()                               end
M.IsItemDeactivated =                 function()               return ImGui.IsItemDeactivated()                           end
M.IsItemDeactivatedAfterEdit =        function()               return ImGui.IsItemDeactivatedAfterEdit()                  end  
M.IsItemClicked =                     function(btn)            return ImGui.IsItemClicked(btn)                            end
M.IsItemEdited =                      function()               return ImGui.IsItemEdited()                                end
M.IsItemToggledOpen =                 function()               return ImGui.IsItemToggledOpen()                           end
                                                                                                                          
M.GetItemRectMin =                    function()               return ImGui.GetItemRectMin()                              end
M.GetItemRectMax =                    function()               return ImGui.GetItemRectMax()                              end
M.GetItemRectSize =                   function()               return ImGui.GetItemRectSize()                             end
                                                                                                                          
M.GetWindowSize =                     function()               return ImGui.GetWindowSize()                               end
M.GetWindowPos =                      function()               return ImGui.GetWindowPos()                                end
                                                                                                                          
M.GetScrollX =                        function()               return ImGui.GetScrollX()                                  end
M.GetScrollY =                        function()               return ImGui.GetScrollY()                                  end
M.GetScrollMaxX =                     function()               return ImGui.GetScrollMaxX()                               end
M.GetScrollMaxY =                     function()               return ImGui.GetScrollMaxY()                               end
M.SetScrollX =                        function(v)              return ImGui.SetScrollX(v)                                 end
M.SetScrollY =                        function(v)              return ImGui.SetScrollY(v)                                 end
M.SetScrollHereX =                    function(xr)             return ImGui.SetScrollHereX(xr or 0.5)                     end
M.SetScrollHereY =                    function(yr)             return ImGui.SetScrollHereY(yr or 0.5)                     end
M.SetScrollFromPosX =                 function(lx, xr)         return ImGui.SetScrollFromPosX(lx, xr or 0.5)              end
M.SetScrollFromPosY =                 function(ly, yr)         return ImGui.SetScrollFromPosY(ly, yr or 0.5)              end
                                                                                                                          
M.TreePop =                           function()               ImGui.TreePop()                                            end
M.TreePush =                          function(id)             ImGui.TreePush(id)                                         end
M.TreeNode =                          function(label)          return ImGui.TreeNode(label)                               end
                                                                                                                          
M.PopButtonRepeat =                   function()               ImGui.PopButtonRepeat()                                    end
M.PushButtonRepeat =                  function(rp)             ImGui.PushButtonRepeat(boolDefault(rp, true))              end
                                                                                                                          
M.PushItemWidth =                     function(val)            ImGui.PushItemWidth(val)                                   end
M.PopItemWidth =                      function()               ImGui.PopItemWidth()                                       end
                                                                                                                          
M.PushStyleColor =                    function(id, val)        ImGui.PushStyleColor(id, val)                              end
M.PopStyleColor =                     function(c)              ImGui.PopStyleColor(c or 1)                                end
                                                                                                                          
M.PushID  =                           function(id)             ImGui.PushID(tostring(id))                                 end
M.PopID =                             function()               ImGui.PopID()                                              end
                                                                                                                          
M.SetTooltip =                        function(tooltip)        ImGui.SetTooltip(tooltip)                                  end
                                                                                                                          
M.CenterNextWindow =                  function()               ImGui.CenterNextWindow()                                   end
M.SetItemDefaultFocus =               function()               ImGui.SetItemDefaultFocus()                                end
M.SetNextItemWidth =                  function(width)          ImGui.SetNextItemWidth(width)                              end
M.SetNextWindowBgAlpha =              function(alpha)          ImGui.SetNextWindowBgAlpha(alpha)                          end
M.SetNextWindowContentSize =          function(sz)             ImGui.SetNextWindowContentSize(sz)                         end
M.GetContentRegionAvail =             function()               return ImGui.GetContentRegionAvail()                       end
                                                                                                                          
M.ColorConvertU32ToFloat4 =           function(col)            return ImGui.ColorConvertU32ToFloat4(col)                  end
M.ColorConvertFloat4ToU32 =           function(col)            return ImGui.ColorConvertFloat4ToU32(col)                  end
                                                                                                                          
M.GetClipboardText =                  function()               return ImGui.GetClipboardText()                            end
M.SetClipboardText =                  function(text)           ImGui.SetClipboardText(text)                               end
                                                                                                                          
M.GetWindowDrawList =                 function()               return ImGui.GetWindowDrawList()                           end
M.GetBackgroundDrawList =             function()               return ImGui.GetBackgroundDrawList()                       end
M.GetForegroundDrawList =             function()               return ImGui.GetForegroundDrawList()                       end
                                                                                                                          
M.GetCursorStartPos =                 function()               return ImGui.GetCursorStartPos()                           end
M.GetCursorScreenPos =                function()               return ImGui.GetCursorScreenPos()                          end
M.GetCursorPos =                      function()               return ImGui.GetCursorPos()                                end
M.GetCursorPosX =                     function()               return ImGui.GetCursorPosX()                               end
M.GetCursorPosY =                     function()               return ImGui.GetCursorPosY()                               end
M.SetCursorPos =                      function(pos)            ImGui.SetCursorPos(pos)                                    end
M.SetCursorPosX =                     function(x)              ImGui.SetCursorPosX(x)                                     end
M.SetCursorPosY =                     function(y)              ImGui.SetCursorPosY(y)                                     end
                                                                                                                          
M.GetIO =                             function()               return ImGui.GetIO()                                       end
M.GetStyle =                          function()               return ImGui.GetStyle()                                    end
                                                                                                                          
-- StackLayout                                                                                                            
M.SuspendLayout =                     function()               ImGui.SuspendLayout()                                      end
M.ResumeLayout =                      function()               ImGui.ResumeLayout()                                       end

--
M.CalcTextSize = function(text, hide_text_after_double_hash, wrap_width)
  local text_end = nullableString(nil) -- automatically calculated, removed from lua side
  hide_text_after_double_hash = boolDefault(hide_text_after_double_hash, false)
  wrap_width = wrap_width or -1.0
  return ImGui.CalcTextSize(text, text_end, hide_text_after_double_hash, wrap_width)
end


M.PopTextWrapPos = function()
  ImGui.PopTextWrapPos()
end

M.PushTextWrapPos = function(wrap_local_pos_x)
  wrap_local_pos_x = wrap_local_pos_x or 0
  ImGui.PushTextWrapPos(wrap_local_pos_x)
end

M.PushStyleVar = function(id, val)  
  --check what variant of the function to use
  if type(val) == 'number' then
    ImGui.PushStyleVarFloat(id, val)
  else
    ImGui.PushStyleVarVec2(id, val)
  end
end

M.PopStyleVar = function(c)
  ImGui.PopStyleVar(c or 1)                               
end

M.BeginTabItem = function(label ,flags)
  flags = flags or 0
  return ImGui.BeginTabItem(label, flags)
end

M.EndTabItem = function()               
  ImGui.EndTabItem()                                      
end

M.BeginTabBar = function(str_id, flags)              
  str_id = str_id or "##TabBar"
  flags = flags or 0
  return ImGui.BeginTabBar(str_id, flags)
end

M.EndTabBar = function()               
  ImGui.EndTabBar()                                      
end

local function plotLinesHistogramSharedSig(label, values, valuesOffset, overlayText, scaleMin, scaleMax, size, stride, func)
  --One scary function signature..
  valuesOffset = valuesOffset or 0
  overlayText = nullableString(overlayText)
  scaleMin = scaleMin or 3.402823466e+38
  scaleMax = scaleMax or 3.402823466e+38
  size = size or ImVec2(0.0, 0.0)
  stride = stride or 4
  func(label, values, valuesOffset, overlayText, scaleMin, scaleMax, size, stride)
end

M.PlotLines = function(label, values, valuesOffset, overlayText, scaleMin, scaleMax, size, stride)
  plotLinesHistogramSharedSig(label, values, valuesOffset, overlayText, scaleMin, scaleMax, size, stride, ImGui.PlotLines)
end

M.PlotHistogram = function(label, values, valuesOffset, overlayText, scaleMin, scaleMax, size, stride)
  plotLinesHistogramSharedSig(label, values, valuesOffset, overlayText, scaleMin, scaleMax, size, stride, ImGui.PlotHistogram)
end

M.InputFloat = function(label, value, step, stepFast, format, flags)
  step = step or 0
  stepFast = stepFast or 0
  format = format or DEFAULT_FLOAT_FORMAT
  flags = flags or 0
  return ImGui.InputFloat(label, value, step, stepFast, format, flags)
end

M.InputFloat2 = function(label, x, y, format, flags)
  format = format or DEFAULT_FLOAT_FORMAT
  flags = flags or 0
  return ImGui.InputFloat2(label, x, y, format, flags)
end

M.InputFloat3 = function(label, x, y, z, format, flags)
  format = format or DEFAULT_FLOAT_FORMAT
  flags = flags or 0
  return ImGui.InputFloat3(label, x, y, z, format, flags)
end

M.InputFloat4 = function(label, x, y, z, w, format, flags)
  format = format or DEFAULT_FLOAT_FORMAT
  flags = flags or 0
  return ImGui.InputFloat4(label, x, y, z, w, format, flags)
end

M.InputInt = function(label, value, step, stepFast, flags)
  step = step or 1
  stepFast = stepFast or 100
  flags = flags or 0
  return ImGui.InputInt(label, value, step, stepFast, flags)
end

M.InputInt2 = function(label, x, y, flags)
  flags = flags or 0
  return ImGui.InputInt2(label, x, y, flags)
end

M.InputInt3 = function(label, x, y, z, flags)
  flags = flags or 0
  return ImGui.InputInt3(label, x, y, z, flags)
end

M.InputInt4 = function(label, x, y, z, w, flags)
  flags = flags or 0
  return ImGui.InputInt4(label, x, y, z, w, flags)
end

M.InputText = function(label, text, flags)
  flags = flags or 0
  return ImGui.InputText(label, text, flags)
end

M.InputTextBoxed = function(label, text, flags)
  flags = flags or 0
  return ImGui.InputTextBoxed(label, text, flags)
end

M.InputTextCallback = function(label, text, flags, cb)
  flags = flags or 0
  return ImGui.InputTextCallback(label, text, flags, cb)
end

M.InputTextBoxedCallback = function(label, text, flags, cb)
  flags = flags or 0
  return ImGui.InputTextBoxedCallback(label, text, flags, cb)
end

M.InputTextMultiline = function(label, text, size, flags)
  size = size or ImVec2(0.0, 0.0)
  flags = flags or 0
  return ImGui.InputTextMultiline(label, text, size, flags)
end

M.InputTextWithHint = function(label, hint, text, flags)
  flags = flags or 0
  return ImGui.InputTextWithHint(label, hint, text, flags)
end

M.SetNextWindowPos = function(pos, cond, pivot)
  pivot = pivot or ImVec2(0.0, 0.0)
  cond = cond or 0
  ImGui.SetNextWindowPos(pos, cond, pivot)
end

M.SetNextWindowSize = function(sz, cond)
  cond = cond or 0
  ImGui.SetNextWindowSize(sz, cond)
end

M.BeginChild = function(str_id, size, border, flags)
  flags = flags or 0
  border = boolDefault(border, true)
  size = size or ImVec2(0, 100)
  return ImGui.BeginChild(str_id, size, border, flags)
end

M.EndChild = function()               
  ImGui.EndChild()                        
end

M.BeginPopupModal = function(name, arg2, arg3)
  -- arg2 is "hasCloseButton" if boolean, otherwise "flags". arg3 is "flags" if "hasCloseButton" is present
  if type(arg2) == 'boolean' then
    arg3 = arg3 or 0
    if arg2 then
      return ImGui.BeginPopupModal(name, arg3)
    else
      return ImGui.BeginPopupModal2(name, arg3)
    end
  elseif type(arg2) == 'number' or type(arg2) == 'nil' then
    arg2 = arg2 or 0
    return ImGui.BeginPopupModal2(name, arg2)
  end
end

M.EndPopup = function()               
  ImGui.EndPopup()                        
end

M.CloseCurrentPopup = function()               
  ImGui.CloseCurrentPopup()                        
end

M.IsPopupOpen = function(name, flags)
  flags = flags or 0
  return ImGui.IsPopupOpen(name, flags)
end

M.OpenPopup = function(name, flags)               
  flags = flags or 0
  ImGui.OpenPopup(name, flags)                       
end

M.Selectable = function(label, selected, flags, size)
  selected = boolDefault(selected, false)
  flags = flags or 0
  size = size or ImVec2(0.0, 0.0)
  
  --Multi return: clicked, selected
  return ImGui.Selectable(label, selected, flags, size)
end

M.MenuItem = function(label, shortcut, selected, enabled)
  selected = boolDefault(selected, false)
  enabled = boolDefault(enabled, true)
  shortcut = nullableString(shortcut)
  
  --Multi return: clicked, selected
  return ImGui.MenuItem(label, shortcut, selected, enabled)
end

M.BeginPopupContextItem = function(str_id, flags)
  return ImGui.BeginPopupContextItem(nullableString(str_id), flags or 0)
end

M.BeginPopupContextWindow = function(str_id, flags)
  return ImGui.BeginPopupContextWindow(nullableString(str_id), flags or 1)
end

M.BeginPopupContextVoid = function(str_id, flags)
  return ImGui.BeginPopupContextVoid(nullableString(str_id), flags or 1)
end

M.EndPopup = function()
  ImGui.EndPopup()
end

M.LeftArrowButton = function()
  return ImGui.LeftArrowButton()
end

M.RightArrowButton = function()
  return ImGui.RightArrowButton()
end

M.UpArrowButton = function()
  return ImGui.UpArrowButton()
end


M.DownArrowButton = function()
  return ImGui.DownArrowButton()
end

M.Unindent = function(indent_w)
  ImGui.Unindent(indent_w or 0)     
end

M.Indent = function(indent_w)
   ImGui.Indent(indent_w or 0)                  
end

M.LeftArrowButton = function()
  return ImGui.LeftArrowButton()
end

M.RightArrowButton = function()
  return ImGui.RightArrowButton()
end

M.UpArrowButton = function()
  return ImGui.UpArrowButton()
end


M.DownArrowButton = function()
  return ImGui.DownArrowButton()
end

M.Checkbox = function(label, checked)
  return ImGui.Checkbox(label, checked)
end

M.RadioButton = function(label, selectedId, id)
  if M.radioButtonGroupOpen then
    id = id or M.radioButtonGroupId
    M.radioButtonGroupId = M.radioButtonGroupId + 1
    
    M.radioButtonGroupSid = ImGui.RadioButton(label, M.radioButtonGroupSid, id)
    return M.radioButtonGroupSid
  else
    return ImGui.radioButton(label, selectedId, id)
  end
end

M.CollapsingHeader = function(label, flags)
  return ImGui.CollapsingHeader(label, flags or 0)
end

M.SameLine = function(offsetX, spacing)
  ImGui.SameLine(offsetX or 0.0, spacing or -1.0)
end

M.Combo = function(label, currentItem, items)
  return ImGui.Combo(label, currentItem, items)
end

M.BeginListBox = function(label, size)
  size = size or ZERO_VEC2
  return ImGui.BeginListBox(label, size)
end

M.EndListBox = function()
  ImGui.EndListBox()
end

M.ListBox = function(label, currentItem, items, heightInItems)
  return ImGui.ListBox(label, currentItem, items, heightInItems or 4)
end

M.VSliderInt = function(label, size, value, min, max, flags)
  min = min or INT_SLIDER_MIN
  max = max or INT_SLIDER_MAX
  flags = flags or 0
  return ImGui.VSliderInt(label, size, value, min, max, flags)
end

M.VSliderFloat = function(label, size, value, min, max, format, flags)
  min = min or FLOAT_SLIDER_MIN
  max = max or FLOAT_SLIDER_MAX
  format = format or DEFAULT_FLOAT_FORMAT
  flags = flags or 0
  return ImGui.VSliderFloat(label, size, value, min, max, format, flags)
end

M.SliderInt = function(label, value, min, max, flags)
  min = min or INT_SLIDER_MIN
  max = max or INT_SLIDER_MAX
  flags = flags or 0
  return ImGui.SliderInt(label, value, min, max, flags)
end

M.SliderInt2 = function(label, x, y, min, max, flags)
  min = min or INT_SLIDER_MIN
  max = max or INT_SLIDER_MAX
  flags = flags or 0
  return ImGui.SliderInt2(label, x, y, min, max, flags)
end

M.SliderInt3 = function(label, x, y, z, min, max, flags)
  min = min or INT_SLIDER_MIN
  max = max or INT_SLIDER_MAX
  flags = flags or 0
  return ImGui.SliderInt3(label, x, y, z, min, max, flags)
end

M.SliderInt4 = function(label, x, y, z, w, min, max, flags)
  min = min or INT_SLIDER_MIN
  max = max or INT_SLIDER_MAX
  flags = flags or 0
  return ImGui.SliderInt4(label, x, y, z, w, min, max, flags)
end

M.SliderAngle = function(label, v_rad, v_degrees_min, v_degrees_max, format, flags)
  v_degrees_min = v_degrees_min or -360
  v_degrees_max = v_degrees_max or 360
  format = format or DEFAULT_ANGLE_FORMAT
  flags = flags or 0
  return ImGui.SliderAngle(label, v_rad, v_degrees_min, v_degrees_max, format, flags)
end

M.SliderFloat = function(label, value, min, max, format, flags)
  min = min or FLOAT_SLIDER_MIN
  max = max or FLOAT_SLIDER_MAX
  format = format or DEFAULT_FLOAT_FORMAT
  flags = flags or 0
  return ImGui.SliderFloat(label, value, min, max, format, flags)
end

M.SliderFloat2 = function(label, x, y, min, max, format, flags)
  min = min or FLOAT_SLIDER_MIN
  max = max or FLOAT_SLIDER_MAX
  format = format or DEFAULT_FLOAT_FORMAT
  flags = flags or 0
  return ImGui.SliderFloat2(label, x, y, min, max, format, flags)
end

M.SliderFloat3 = function(label, x, y, z, min, max, format, flags)
  min = min or FLOAT_SLIDER_MIN
  max = max or FLOAT_SLIDER_MAX
  format = format or DEFAULT_FLOAT_FORMAT
  flags = flags or 0
  return ImGui.SliderFloat3(label, x, y, z, min, max, format, flags)
end

M.SliderFloat4 = function(label, x, y, z, w, min, max, format, flags)
  min = min or FLOAT_SLIDER_MIN
  max = max or FLOAT_SLIDER_MAX
  flags = flags or 0
  format = format or DEFAULT_FLOAT_FORMAT
  return ImGui.SliderFloat4(label, x, y, z, w, min, max, format, flags)
end

M.Slider = function(label, value, min, max)
  --Compatibility function, the name "Slider" was used
  --During development, but is now supposed to be SliderFloat
  return M.SliderFloat(label, value, min, max)
end

M.DragFloat4 = function(label, x, y, z, w, speed, min, max, format, flags)
  min = min or 0
  max = max or 0
  speed = speed or 1
  format = format or DEFAULT_FLOAT_FORMAT
  flags = flags or 0
  return ImGui.DragFloat4(label, x, y, z, w, speed, min, max, format, flags)
end

M.DragFloat2 = function(label, x, y, speed, min, max, format, flags)
  min = min or 0
  max = max or 0
  speed = speed or 1
  format = format or DEFAULT_FLOAT_FORMAT
  flags = flags or 0
  return ImGui.DragFloat2(label, x, y, speed, min, max, format, flags)
end

M.DragFloat3 = function(label, x, y, z, speed, min, max, format, flags)
  min = min or 0
  max = max or 0
  speed = speed or 1
  format = format or DEFAULT_FLOAT_FORMAT
  flags = flags or 0
  return ImGui.DragFloat3(label, x, y, z, speed, min, max, format, flags)
end

M.DragInt4 = function(label, x, y, z, w, speed, min, max, flags)
  min = min or 0
  max = max or 0
  speed = speed or 1
  flags = flags or 0
  return ImGui.DragInt4(label, x, y, z, w, speed, min, max, flags)
end

M.DragInt3 = function(label, x, y, z, speed, min, max, flags)
  min = min or 0
  max = max or 0
  speed = speed or 1
  flags = flags or 0
  return ImGui.DragInt3(label, x, y, z, speed, min, max, flags)
end

M.DragInt2 = function(label, x, y, speed, min, max, flags)
  min = min or 0
  max = max or 0
  speed = speed or 1
  flags = flags or 0
  return ImGui.DragInt2(label, x, y, speed, min, max, flags)
end

M.ColorButton = function(dec_id, r, g, b, a, flags, size)
  flags = flags or 0
  size = size or ZERO_VEC2
  a = a or 1.0
  return ImGui.ColorButton(dec_id, r, g, b, a, flags, size)
end

M.ColorEdit4 = function(label, r, g, b, a, flags)
  flags = flags or 0
  return ImGui.ColorEdit4(label, r, g, b, a, flags)
end

M.ColorEdit3 = function(label, r, g, b, flags)
  flags = flags or 0
  return ImGui.ColorEdit3(label, r, g, b, flags)
end

M.DragInt = function(label, value, speed, min, max, flags)
  min = min or 0
  max = max or 0
  speed = speed or 1
  flags = flags or 0
  return ImGui.DragInt(label, value, speed, min, max, flags)
end

M.DragFloat = function(label, value, speed, min, max, format, flags)
  min = min or 0
  max = max or 0
  speed = speed or 1
  format = format or DEFAULT_FLOAT_FORMAT
  flags = flags or 0
  return ImGui.DragFloat(label, value, speed, min, max, format, flags)
end

M.Drag = function(label, value, speed, min, max)
  --Compatibility function, the name "Drag" was used
  --During development, but is now supposed to be DragFloat
  return M.DragFloat(label, value, speed, min, max)
end

M.End = function()
  ImGui.End()
end

M.Begin = function(name, arg2, arg3)
  -- arg2 is "hasCloseButton" if boolean, otherwise "flags". arg3 is "flags" if "hasCloseButton" is present
  if type(arg2) == 'boolean' then
    arg3 = arg3 or 0
    if arg2 then
      return ImGui.Begin(name, arg3)
    else
      return ImGui.Begin2(name, arg3)
    end
  elseif type(arg2) == 'number' or type(arg2) == 'nil' then
    arg2 = arg2 or 0
    return ImGui.Begin2(name, arg2)
  end
end

M.InvisibleButton = function(text, size, flags)
  flags = flags or 0
  return ImGui.InvisibleButton(text, size, flags) 
end

M.Button = function(text, size)
  return ImGui.Button(text, size or ZERO_VEC2)
end

M.SmallButton = function(text)
  return ImGui.SmallButton(text)
end

M.ProgressBar = function(fraction, size, overlay)
  size = size or ImVec2(-1, 0)
  overlay = nullableString(overlay)
  ImGui.ProgressBar(fraction, size, overlay)
end

M.Image = function(texture, size, uv0, uv1, tintColor, borderColor)
  uv0 = uv0 or ZERO_VEC2
  uv1 = uv1 or ImVec2(1, 1)
  tintColor = tintColor or ImVec4(1, 1, 1, 1)
  borderColor = borderColor or ImVec4(0, 0, 0, 0)
  ImGui.Image(texture, size, uv0, uv1, tintColor, borderColor)
end

M.Columns = function(count, id, border)
  count = count or 1
  id = nullableString(id)
  border = boolDefault(border, true)
  ImGui.Columns(count, id, border)
end

M.Tooltip = function(text)
  if M.IsItemHovered() then
      M.BeginTooltip()
      M.PushTextWrapPos(450.0)
      M.Text(text)
      M.PopTextWrapPos()
      M.EndTooltip()
  end
end

--StackLayout
M.BeginHorizontal = function(id, size, align)
  size = size or ZERO_VEC2
  align = align or -1.0
  ImGui.BeginHorizontal(id, size, align)
end

M.BeginVertical = function(id, size, align)
  size = size or ZERO_VEC2
  align = align or -1.0
  ImGui.BeginVertical(id, size, align)
end

M.EndHorizontal = function()
  ImGui.EndHorizontal()
end

M.EndVertical = function()
  ImGui.EndVertical()
end

M.Spring = function(weight, spacing)
  weight = weight or 1.0
  spacing = spacing or -1.0
  ImGui.Spring(weight, spacing)
end

--Demo window. Draw this for cool examples
M.ShowDemoWindow = function(show) 
  show = boolDefault(show, true)
  return ImGui.ShowDemoWindow(show)
end

--Make global constants
ImGuiColorEditFlags_None                    = 0
ImGuiColorEditFlags_NoAlpha                 = 1 << 1
ImGuiColorEditFlags_NoPicker                = 1 << 2
ImGuiColorEditFlags_NoOptions               = 1 << 3
ImGuiColorEditFlags_NoSmallPreview          = 1 << 4
ImGuiColorEditFlags_NoInputs                = 1 << 5
ImGuiColorEditFlags_NoTooltip               = 1 << 6
ImGuiColorEditFlags_NoLabel                 = 1 << 7
ImGuiColorEditFlags_NoSidePreview           = 1 << 8
ImGuiColorEditFlags_NoDragDrop              = 1 << 9
ImGuiColorEditFlags_NoBorder                = 1 << 10
ImGuiColorEditFlags_AlphaBar                = 1 << 16
ImGuiColorEditFlags_AlphaPreview            = 1 << 17
ImGuiColorEditFlags_AlphaPreviewHalf        = 1 << 18
ImGuiColorEditFlags_HDR                     = 1 << 19
ImGuiColorEditFlags_DisplayRGB              = 1 << 20
ImGuiColorEditFlags_DisplayHSV              = 1 << 21
ImGuiColorEditFlags_DisplayHex              = 1 << 22
ImGuiColorEditFlags_Uint8                   = 1 << 23
ImGuiColorEditFlags_Float                   = 1 << 24
ImGuiColorEditFlags_PickerHueBar            = 1 << 25
ImGuiColorEditFlags_PickerHueWheel          = 1 << 26
ImGuiColorEditFlags_InputRGB                = 1 << 27
ImGuiColorEditFlags_InputHSV                = 1 << 28
    
ImGuiStyleVar_Alpha                         = 0
ImGuiStyleVar_DisabledAlpha                 = 1
ImGuiStyleVar_WindowPadding                 = 2
ImGuiStyleVar_WindowRounding                = 3
ImGuiStyleVar_WindowBorderSize              = 4
ImGuiStyleVar_WindowMinSize                 = 5
ImGuiStyleVar_WindowTitleAlign              = 6
ImGuiStyleVar_ChildRounding                 = 7
ImGuiStyleVar_ChildBorderSize               = 8
ImGuiStyleVar_PopupRounding                 = 9
ImGuiStyleVar_PopupBorderSize               = 10
ImGuiStyleVar_FramePadding                  = 11
ImGuiStyleVar_FrameRounding                 = 12
ImGuiStyleVar_FrameBorderSize               = 13
ImGuiStyleVar_ItemSpacing                   = 14
ImGuiStyleVar_ItemInnerSpacing              = 15
ImGuiStyleVar_IndentSpacing                 = 16
ImGuiStyleVar_CellPadding                   = 17
ImGuiStyleVar_ScrollbarSize                 = 18
ImGuiStyleVar_ScrollbarRounding             = 19
ImGuiStyleVar_GrabMinSize                   = 20
ImGuiStyleVar_GrabRounding                  = 21
ImGuiStyleVar_TabRounding                   = 22
ImGuiStyleVar_ButtonTextAlign               = 23
ImGuiStyleVar_SelectableTextAlign           = 24
ImGuiStyleVar_SeparatorTextBorderSize       = 25
ImGuiStyleVar_SeparatorTextAlign            = 26
ImGuiStyleVar_SeparatorTextPadding          = 27
ImGuiStyleVar_COUNT                         = 28

ImGuiCol_Text                               = 0
ImGuiCol_TextDisabled                       = 1
ImGuiCol_WindowBg                           = 2
ImGuiCol_ChildBg                            = 3
ImGuiCol_PopupBg                            = 4
ImGuiCol_Border                             = 5
ImGuiCol_BorderShadow                       = 6
ImGuiCol_FrameBg                            = 7
ImGuiCol_FrameBgHovered                     = 8
ImGuiCol_FrameBgActive                      = 9
ImGuiCol_TitleBg                            = 10
ImGuiCol_TitleBgActive                      = 11
ImGuiCol_TitleBgCollapsed                   = 12
ImGuiCol_MenuBarBg                          = 13
ImGuiCol_ScrollbarBg                        = 14
ImGuiCol_ScrollbarGrab                      = 15
ImGuiCol_ScrollbarGrabHovered               = 16
ImGuiCol_ScrollbarGrabActive                = 17
ImGuiCol_CheckMark                          = 18
ImGuiCol_SliderGrab                         = 19
ImGuiCol_SliderGrabActive                   = 20
ImGuiCol_Button                             = 21
ImGuiCol_ButtonHovered                      = 22
ImGuiCol_ButtonActive                       = 23
ImGuiCol_Header                             = 24
ImGuiCol_HeaderHovered                      = 25
ImGuiCol_HeaderActive                       = 26
ImGuiCol_Separator                          = 27
ImGuiCol_SeparatorHovered                   = 28
ImGuiCol_SeparatorActive                    = 29
ImGuiCol_ResizeGrip                         = 30
ImGuiCol_ResizeGripHovered                  = 31
ImGuiCol_ResizeGripActive                   = 32
ImGuiCol_Tab                                = 33
ImGuiCol_TabHovered                         = 34
ImGuiCol_TabActive                          = 35
ImGuiCol_TabUnfocused                       = 36
ImGuiCol_TabUnfocusedActive                 = 37
ImGuiCol_DockingPreview                     = 38
ImGuiCol_DockingEmptyBg                     = 39
ImGuiCol_PlotLines                          = 40
ImGuiCol_PlotLinesHovered                   = 41
ImGuiCol_PlotHistogram                      = 42
ImGuiCol_PlotHistogramHovered               = 43
ImGuiCol_TableHeaderBg                      = 44
ImGuiCol_TableBorderStrong                  = 45
ImGuiCol_TableBorderLight                   = 46
ImGuiCol_TableRowBg                         = 47
ImGuiCol_TableRowBgAlt                      = 48
ImGuiCol_TextSelectedBg                     = 49
ImGuiCol_DragDropTarget                     = 50
ImGuiCol_NavHighlight                       = 51
ImGuiCol_NavWindowingHighlight              = 52
ImGuiCol_NavWindowingDimBg                  = 53
ImGuiCol_ModalWindowDimBg                   = 54
ImGuiCol_COUNT                              = 55

ImGuiWindowFlags_None                       = 0
ImGuiWindowFlags_NoTitleBar                 = 1 << 0
ImGuiWindowFlags_NoResize                   = 1 << 1
ImGuiWindowFlags_NoMove                     = 1 << 2
ImGuiWindowFlags_NoScrollbar                = 1 << 3
ImGuiWindowFlags_NoScrollWithMouse          = 1 << 4
ImGuiWindowFlags_NoCollapse                 = 1 << 5
ImGuiWindowFlags_AlwaysAutoResize           = 1 << 6
ImGuiWindowFlags_NoBackground               = 1 << 7
ImGuiWindowFlags_NoSavedSettings            = 1 << 8
ImGuiWindowFlags_NoMouseInputs              = 1 << 9
ImGuiWindowFlags_MenuBar                    = 1 << 10
ImGuiWindowFlags_HorizontalScrollbar        = 1 << 11
ImGuiWindowFlags_NoFocusOnAppearing         = 1 << 12
ImGuiWindowFlags_NoBringToFrontOnFocus      = 1 << 13
ImGuiWindowFlags_AlwaysVerticalScrollbar    = 1 << 14
ImGuiWindowFlags_AlwaysHorizontalScrollbar  = 1<< 15
ImGuiWindowFlags_AlwaysUseWindowPadding     = 1 << 16
ImGuiWindowFlags_NoNavInputs                = 1 << 18
ImGuiWindowFlags_NoNavFocus                 = 1 << 19
ImGuiWindowFlags_UnsavedDocument            = 1 << 20
ImGuiWindowFlags_NoDocking                  = 1 << 21
ImGuiWindowFlags_NoNav                      = ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus
ImGuiWindowFlags_NoDecoration               = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse
ImGuiWindowFlags_NoInputs                   = ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus

ImGuiCond_None          = 0   
ImGuiCond_Always        = 1 << 0
ImGuiCond_Once          = 1 << 1
ImGuiCond_FirstUseEver  = 1 << 2
ImGuiCond_Appearing     = 1 << 3

ImGuiInputTextFlags_None                    = 0
ImGuiInputTextFlags_CharsDecimal            = 1 << 0
ImGuiInputTextFlags_CharsHexadecimal        = 1 << 1
ImGuiInputTextFlags_CharsUppercase          = 1 << 2
ImGuiInputTextFlags_CharsNoBlank            = 1 << 3
ImGuiInputTextFlags_AutoSelectAll           = 1 << 4
ImGuiInputTextFlags_EnterReturnsTrue        = 1 << 5
ImGuiInputTextFlags_CallbackCompletion      = 1 << 6
ImGuiInputTextFlags_CallbackHistory         = 1 << 7
ImGuiInputTextFlags_CallbackAlways          = 1 << 8
ImGuiInputTextFlags_CallbackCharFilter      = 1 << 9
ImGuiInputTextFlags_AllowTabInput           = 1 << 10
ImGuiInputTextFlags_CtrlEnterForNewLine     = 1 << 11
ImGuiInputTextFlags_NoHorizontalScroll      = 1 << 12
ImGuiInputTextFlags_AlwaysOverwrite         = 1 << 13
ImGuiInputTextFlags_ReadOnly                = 1 << 14
ImGuiInputTextFlags_Password                = 1 << 15
ImGuiInputTextFlags_NoUndoRedo              = 1 << 16
ImGuiInputTextFlags_CharsScientific         = 1 << 17
ImGuiInputTextFlags_CallbackResize          = 1 << 18
ImGuiInputTextFlags_CallbackEdit            = 1 << 19
ImGuiInputTextFlags_EscapeClearsAll         = 1 << 20

ImGuiSliderFlags_None                   = 0
ImGuiSliderFlags_AlwaysClamp            = 1 << 4 
ImGuiSliderFlags_Logarithmic            = 1 << 5 
ImGuiSliderFlags_NoRoundToFormat        = 1 << 6 
ImGuiSliderFlags_NoInput                = 1 << 7 

ImGuiButtonFlags_None                   = 0
ImGuiButtonFlags_MouseButtonLeft        = 1 << 0
ImGuiButtonFlags_MouseButtonRight       = 1 << 1
ImGuiButtonFlags_MouseButtonMiddle      = 1 << 2

ImGuiTabBarFlags_None                           = 0
ImGuiTabBarFlags_Reorderable                    = 1 << 0
ImGuiTabBarFlags_AutoSelectNewTabs              = 1 << 1
ImGuiTabBarFlags_TabListPopupButton             = 1 << 2
ImGuiTabBarFlags_NoCloseWithMiddleMouseButton   = 1 << 3
ImGuiTabBarFlags_NoTabListScrollingButtons      = 1 << 4
ImGuiTabBarFlags_NoTooltip                      = 1 << 5
ImGuiTabBarFlags_FittingPolicyResizeDown        = 1 << 6
ImGuiTabBarFlags_FittingPolicyScroll            = 1 << 7

ImGuiTabItemFlags_None                          = 0
ImGuiTabItemFlags_UnsavedDocument               = 1 << 0
ImGuiTabItemFlags_SetSelected                   = 1 << 1
ImGuiTabItemFlags_NoCloseWithMiddleMouseButton  = 1 << 2
ImGuiTabItemFlags_NoPushId                      = 1 << 3
ImGuiTabItemFlags_NoTooltip                     = 1 << 4
ImGuiTabItemFlags_NoReorder                     = 1 << 5
ImGuiTabItemFlags_Leading                       = 1 << 6
ImGuiTabItemFlags_Trailing                      = 1 << 7

ImGuiMouseButton_Left                           = 0
ImGuiMouseButton_Right                          = 1
ImGuiMouseButton_Middle                         = 2

ImGuiHoveredFlags_None                      = 0
ImGuiHoveredFlags_ChildWindows              = 1 << 0
ImGuiHoveredFlags_RootWindow                = 1 << 1
ImGuiHoveredFlags_AnyWindow                 = 1 << 2
ImGuiHoveredFlags_NoPopupHierarchy          = 1 << 3
ImGuiHoveredFlags_DockHierarchy             = 1 << 4
ImGuiHoveredFlags_AllowWhenBlockedByPopup   = 1 << 5
--ImGuiHoveredFlags_AllowWhenBlockedByModal = 1 << 6
ImGuiHoveredFlags_AllowWhenBlockedByActiveItem= 1 << 7
ImGuiHoveredFlags_AllowWhenOverlapped       = 1 << 8
ImGuiHoveredFlags_AllowWhenDisabled         = 1 << 9
ImGuiHoveredFlags_NoNavOverride             = 1 << 10
ImGuiHoveredFlags_RectOnly                  = ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenOverlapped
ImGuiHoveredFlags_RootAndChildWindows       = ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows
ImGuiHoveredFlags_DelayNormal               = 1 << 11
ImGuiHoveredFlags_DelayShort                = 1 << 12
ImGuiHoveredFlags_NoSharedDelay             = 1 << 13
    
ImGuiFocusedFlags_None                      = 0
ImGuiFocusedFlags_ChildWindows              = 1 << 0
ImGuiFocusedFlags_RootWindow                = 1 << 1
ImGuiFocusedFlags_AnyWindow                 = 1 << 2
ImGuiFocusedFlags_NoPopupHierarchy          = 1 << 3
ImGuiFocusedFlags_DockHierarchy             = 1 << 4
ImGuiFocusedFlags_RootAndChildWindows       = ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_ChildWindows

ImGuiTreeNodeFlags_None                     = 0
ImGuiTreeNodeFlags_Selected                 = 1 << 0
ImGuiTreeNodeFlags_Framed                   = 1 << 1
ImGuiTreeNodeFlags_AllowItemOverlap         = 1 << 2
ImGuiTreeNodeFlags_NoTreePushOnOpen         = 1 << 3
ImGuiTreeNodeFlags_NoAutoOpenOnLog          = 1 << 4
ImGuiTreeNodeFlags_DefaultOpen              = 1 << 5
ImGuiTreeNodeFlags_OpenOnDoubleClick        = 1 << 6
ImGuiTreeNodeFlags_OpenOnArrow              = 1 << 7
ImGuiTreeNodeFlags_Leaf                     = 1 << 8
ImGuiTreeNodeFlags_Bullet                   = 1 << 9
ImGuiTreeNodeFlags_FramePadding             = 1 << 10
ImGuiTreeNodeFlags_SpanAvailWidth           = 1 << 11
ImGuiTreeNodeFlags_SpanFullWidth            = 1 << 12
ImGuiTreeNodeFlags_NavLeftJumpsBackHere     = 1 << 13
ImGuiTreeNodeFlags_CollapsingHeader         = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_NoAutoOpenOnLog

ImDrawListFlags_None                    = 0
ImDrawListFlags_AntiAliasedLines        = 1 << 0
ImDrawListFlags_AntiAliasedLinesUseTex  = 1 << 1
ImDrawListFlags_AntiAliasedFill         = 1 << 2
ImDrawListFlags_AllowVtxOffset          = 1 << 3

ImDrawFlags_None                        = 0
ImDrawFlags_Closed                      = 1 << 0
ImDrawFlags_RoundCornersTopLeft         = 1 << 4
ImDrawFlags_RoundCornersTopRight        = 1 << 5
ImDrawFlags_RoundCornersBottomLeft      = 1 << 6
ImDrawFlags_RoundCornersBottomRight     = 1 << 7
ImDrawFlags_RoundCornersNone            = 1 << 8
ImDrawFlags_RoundCornersTop             = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight
ImDrawFlags_RoundCornersBottom          = ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersBottomRight
ImDrawFlags_RoundCornersLeft            = ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersTopLeft
ImDrawFlags_RoundCornersRight           = ImDrawFlags_RoundCornersBottomRight | ImDrawFlags_RoundCornersTopRight

ImGuiKey_Tab                 =512
ImGuiKey_LeftArrow           =513
ImGuiKey_RightArrow          =514
ImGuiKey_UpArrow             =515
ImGuiKey_DownArrow           =516
ImGuiKey_PageUp              =517
ImGuiKey_PageDown            =518
ImGuiKey_Home                =519
ImGuiKey_End                 =520
ImGuiKey_Insert              =521
ImGuiKey_Delete              =522
ImGuiKey_Backspace           =523
ImGuiKey_Space               =524
ImGuiKey_Enter               =525
ImGuiKey_Escape              =526
ImGuiKey_LeftCtrl            =527
ImGuiKey_LeftShift           =528
ImGuiKey_LeftAlt             =529
ImGuiKey_LeftSuper           =530
ImGuiKey_RightCtrl           =531
ImGuiKey_RightShift          =532
ImGuiKey_RightAlt            =533
ImGuiKey_RightSuper          =534
ImGuiKey_Menu                =535
ImGuiKey_0                   =536
ImGuiKey_1                   =537
ImGuiKey_2                   =538
ImGuiKey_3                   =539
ImGuiKey_4                   =540
ImGuiKey_5                   =541
ImGuiKey_6                   =542
ImGuiKey_7                   =543
ImGuiKey_8                   =544
ImGuiKey_9                   =545
ImGuiKey_A                   =546
ImGuiKey_B                   =547
ImGuiKey_C                   =548
ImGuiKey_D                   =549
ImGuiKey_E                   =550
ImGuiKey_F                   =551
ImGuiKey_G                   =552
ImGuiKey_H                   =553
ImGuiKey_I                   =554
ImGuiKey_J                   =555
ImGuiKey_K                   =556
ImGuiKey_L                   =557
ImGuiKey_M                   =558
ImGuiKey_N                   =559
ImGuiKey_O                   =560
ImGuiKey_P                   =561
ImGuiKey_Q                   =562
ImGuiKey_R                   =563
ImGuiKey_S                   =564
ImGuiKey_T                   =565
ImGuiKey_U                   =566
ImGuiKey_V                   =567
ImGuiKey_W                   =568
ImGuiKey_X                   =569
ImGuiKey_Y                   =570
ImGuiKey_Z                   =571
ImGuiKey_F1                  =572
ImGuiKey_F2                  =573
ImGuiKey_F3                  =574
ImGuiKey_F4                  =575
ImGuiKey_F5                  =576
ImGuiKey_F6                  =577
ImGuiKey_F7                  =578
ImGuiKey_F8                  =579
ImGuiKey_F9                  =580
ImGuiKey_F10                 =581
ImGuiKey_F11                 =582
ImGuiKey_F12                 =583
ImGuiKey_Apostrophe          =584
ImGuiKey_Comma               =585
ImGuiKey_Minus               =586
ImGuiKey_Period              =587
ImGuiKey_Slash               =588
ImGuiKey_Semicolon           =589
ImGuiKey_Equal               =590
ImGuiKey_LeftBracket         =591
ImGuiKey_Backslash           =592
ImGuiKey_RightBracket        =593
ImGuiKey_GraveAccent         =594
ImGuiKey_CapsLock            =595
ImGuiKey_ScrollLock          =596
ImGuiKey_NumLock             =597
ImGuiKey_PrintScreen         =598
ImGuiKey_Pause               =599
ImGuiKey_Keypad0             =600
ImGuiKey_Keypad1             =601
ImGuiKey_Keypad2             =602
ImGuiKey_Keypad3             =603
ImGuiKey_Keypad4             =604
ImGuiKey_Keypad5             =605
ImGuiKey_Keypad6             =606
ImGuiKey_Keypad7             =607
ImGuiKey_Keypad8             =608
ImGuiKey_Keypad9             =609
ImGuiKey_KeypadDecimal       =610
ImGuiKey_KeypadDivide        =611
ImGuiKey_KeypadMultiply      =612
ImGuiKey_KeypadSubtract      =613
ImGuiKey_KeypadAdd           =614
ImGuiKey_KeypadEnter         =615
ImGuiKey_KeypadEqual         =616
ImGuiKey_GamepadStart        =617
ImGuiKey_GamepadBack         =618
ImGuiKey_GamepadFaceLeft     =619
ImGuiKey_GamepadFaceRight    =620
ImGuiKey_GamepadFaceUp       =621
ImGuiKey_GamepadFaceDown     =622
ImGuiKey_GamepadDpadLeft     =623
ImGuiKey_GamepadDpadRight    =624
ImGuiKey_GamepadDpadUp       =625
ImGuiKey_GamepadDpadDown     =626
ImGuiKey_GamepadL1           =627
ImGuiKey_GamepadR1           =628
ImGuiKey_GamepadL2           =629
ImGuiKey_GamepadR2           =630
ImGuiKey_GamepadL3           =631
ImGuiKey_GamepadR3           =632
ImGuiKey_GamepadLStickLeft   =633
ImGuiKey_GamepadLStickRight  =634
ImGuiKey_GamepadLStickUp     =635
ImGuiKey_GamepadLStickDown   =636
ImGuiKey_GamepadRStickLeft   =637
ImGuiKey_GamepadRStickRight  =638
ImGuiKey_GamepadRStickUp     =639
ImGuiKey_GamepadRStickDown   =640
ImGuiKey_MouseLeft           =641
ImGuiKey_MouseRight          =642
ImGuiKey_MouseMiddle         =643
ImGuiKey_MouseX1             =644
ImGuiKey_MouseX2             =645
ImGuiKey_MouseWheelX         =646
ImGuiKey_MouseWheelY         =647

return M