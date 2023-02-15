local M = {}

--IMGUI Layer that provides default parameters etc. For your sanity.
--Forwarded functions are on a single line. (with some exceptions like push/pop, and multiple possible default vals) 
--Other functions have multi line bodies.

--Internal Constants
local INT_SLIDER_MIN = -2147483648
local INT_SLIDER_MAX = 2147483647
local FLOAT_SLIDER_MIN = -3.40282347e+38 / 2
local FLOAT_SLIDER_MAX = 3.40282347e+38 / 2

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
M.BeginTooltip =                      function()               ImGui.BeginTooltip()                                    end
M.EndTooltip =                        function()               ImGui.EndTooltip()                                      end
                    
M.BeginMenu =                         function(mNam, enable)   return ImGui.BeginMenu(mNam, boolDefault(enable, true)) end
M.EndMenu =                           function()               ImGui.EndMenu()                                         end
M.BeginMenuBar =                      function()               return ImGui.BeginMenuBar()                             end
M.EndMenuBar =                        function()               ImGui.EndMenuBar()                                      end
M.BeginMainMenuBar =                  function()               return ImGui.BeginMainMenuBar()                         end
M.EndMainMenuBar =                    function()               ImGui.EndMainMenuBar()                                  end
          
M.Dummy =                             function(sz)             ImGui.Dummy(sz or ImVec2(0, 0))                         end        

M.BeginDisabled =                     function(disabled)       ImGui.BeginDisabled(boolDefault(disabled, true))        end
M.EndDisabled =                       function()               ImGui.EndDisabled()                                     end
          
M.HelpMarker =                        function(text)           ImGui.HelpMarker(text)                                  end
M.Separator =                         function()               ImGui.Separator()                                       end
M.SeparatorText =                     function(text)               ImGui.SeparatorText(text)                                       end
                
M.AlignTextToFramePadding =           function()               ImGui.AlignTextToFramePadding()                         end
M.GetTextLineHeight =                 function()               return ImGui.GetTextLineHeight()                        end
M.CalcItemWidth =                     function()               return ImGui.CalcItemWidth()                            end
M.GetFrameCount =                     function()               return ImGui.GetFrameCount()                            end
M.GetFrameHeight =                    function()               return ImGui.GetFrameHeight()                           end
M.GetFrameHeightWithSpacing =         function()               return ImGui.GetFrameHeightWithSpacing()                end
M.Text =                              function(text)           ImGui.Text(text)                                        end
M.TextWrapped =                       function(text)           ImGui.TextWrapped(text)                                 end
M.TextColored =                       function(color, text)    ImGui.TextColored(color, text)                          end
M.LabelText =                         function(label, text)    ImGui.LabelText(label, text)                            end
M.BulletText =                        function(text)           ImGui.BulletText(text)                                  end

M.GetFont =                           function()               return ImGui.GetFont()                                  end       
M.GetFontSize =                       function()               return ImGui.GetFontSize()                              end
M.SetWindowFontScale =                function(scale)          ImGui.SetWindowFontScale(scale)                         end       
M.PushFont =                          function(font)           ImGui.PushFont(font)                                    end       
M.PopFont =                           function()               ImGui.PopFont()                                         end       

M.GetColumnIndex =                    function()               return ImGui.GetColumnIndex()                           end
M.GetColumnOffset =                   function(index)          return ImGui.GetColumnOffset(index or -1)               end
M.GetColumnsCount =                   function()               return ImGui.GetColumnsCount()                          end
M.GetColumnWidth =                    function(index)          return ImGui.GetColumnWidth(index or -1)                end
M.NextColumn =                        function()               ImGui.NextColumn()                                      end
M.SetColumnOffset =                   function(index, offset)  ImGui.SetColumnOffset(index, offset)                    end
M.SetColumnWidth =                    function(index, width)   ImGui.SetColumnWidth(index, width)                      end

M.GetMousePos =                       function()               return ImGui.GetMousePos()                              end        
M.GetMousePosOnOpeningCurrentPopup =  function()               return ImGui.GetMousePosOnOpeningCurrentPopup()         end        
M.IsMouseDown =                       function(btn)            return ImGui.IsMouseDown(btn)                           end
M.IsMouseClicked =                    function(btn,rp)         return ImGui.IsMouseClicked(btn, boolDefault(rp,false)) end
M.IsMouseReleased =                   function(btn)            return ImGui.IsMouseReleased(btn)                       end
M.IsAnyMouseDown =                    function()               return ImGui.IsAnyMouseDown()                           end
M.IsMouseDragging =                   function(btn, lockThrsh) return ImGui.IsMouseDragging(btn. lockThrsh or -1.0)    end
M.GetMouseDragDelta =                 function(btn, lockThrsh) return ImGui.GetMouseDragDelta(btn, lockThrsh or -1.0)  end

M.IsWindowFocused =                   function(focusFlags)     return ImGui.IsWindowFocused(focusFlags or 0)           end
M.IsWindowHovered =                   function(hoverFlags)     return ImGui.IsWindowHovered(hoverFlags or 0)           end
M.IsItemActive =                      function()               return ImGui.IsItemActive()                             end
M.IsItemFocused =                     function()               return ImGui.IsItemFocused()                            end
M.IsItemHovered =                     function(hoverFlags)     return ImGui.IsItemHovered(hoverFlags or 0)             end
M.IsAnyItemActive =                   function()               return ImGui.IsAnyItemActive()                          end
M.IsAnyItemFocused =                  function()               return ImGui.IsAnyItemFocused()                         end
M.IsAnyItemHovered =                  function()               return ImGui.IsAnyItemHovered()                         end
M.IsItemActivated =                   function()               return ImGui.IsItemActivated()                          end
M.IsItemVisible =                     function()               return ImGui.IsItemVisible()                            end
M.IsItemDeactivated =                 function()               return ImGui.IsItemDeactivated()                        end
M.IsItemDeactivatedAfterEdit =        function()               return ImGui.IsItemDeactivatedAfterEdit()               end  
M.IsItemClicked =                     function(btn)            return ImGui.IsItemClicked(btn)                         end
M.IsItemEdited =                      function()               return ImGui.IsItemEdited()                             end
M.IsItemToggledOpen =                 function()               return ImGui.IsItemToggledOpen()                        end

M.GetItemRectMin =                    function()               return ImGui.GetItemRectMin()                           end
M.GetItemRectMax =                    function()               return ImGui.GetItemRectMax()                           end
M.GetItemRectSize =                   function()               return ImGui.GetItemRectSize()                          end

M.GetScrollX =                        function()               return ImGui.GetScrollX()                               end
M.GetScrollY =                        function()               return ImGui.GetScrollY()                               end
M.GetScrollMaxX =                     function()               return ImGui.GetScrollMaxX()                            end
M.GetScrollMaxY =                     function()               return ImGui.GetScrollMaxY()                            end
M.SetScrollHereX =                    function(xr)             return ImGui.SetScrollHereX(xr or 0.5)                  end
M.SetScrollHereY =                    function(yr)             return ImGui.SetScrollHereY(yr or 0.5)                  end
M.SetScrollFromPosX =                 function(lx, xr)         return ImGui.SetScrollFromPosX(lx, xr or 0.5)           end
M.SetScrollFromPosY =                 function(ly, yr)         return ImGui.SetScrollFromPosY(ly, yr or 0.5)           end

M.TreePop =                           function()               ImGui.TreePop()                                         end
M.TreeNode =                          function(label)          return ImGui.TreeNode(label)                            end
                
M.PopButtonRepeat =                   function()               ImGui.PopButtonRepeat()                                 end
M.PushButtonRepeat =                  function(rp)             ImGui.PushButtonRepeat(boolDefault(rp, true))           end
          
M.PushItemWidth =                     function(val)            ImGui.PushItemWidth(val)                                end
M.PopItemWidth =                      function()               ImGui.PopItemWidth()                                    end
                              
M.PushStyleColor =                    function(id, val)        ImGui.PushStyleColor(id, val)                           end
M.PopStyleColor =                     function(c)              ImGui.PopStyleColor(c or 1)                             end
                
M.SetTooltip =                        function(tooltip)        ImGui.SetTooltip(tooltip)                               end
                
M.CenterNextWindow =                  function()               ImGui.CenterNextWindow()                                end
M.SetItemDefaultFocus =               function()               ImGui.SetItemDefaultFocus()                             end
M.SetNextItemWidth =                  function(width)          ImGui.SetNextItemWidth(width)                           end
M.SetNextWindowBgAlpha =              function(alpha)          ImGui.SetNextWindowBgAlpha(alpha)                       end
M.SetNextWindowContentSize =          function(sz)             ImGui.SetNextWindowContentSize(sz)                      end

M.ColorConvertU32ToFloat4 =           function(col)            return ImGui.ColorConvertU32ToFloat4(col)               end
M.ColorConvertFloat4ToU32 =           function(col)            return ImGui.ColorConvertFloat4ToU32(col)               end

M.GetClipboardText =                  function()               return ImGui.GetClipboardText()                         end
M.SetClipboardText =                  function(text)           ImGui.SetClipboardText(text)                            end

M.GetWindowDrawList =                 function()               return ImGui.GetWindowDrawList()                        end

M.GetCursorStartPos =                 function()               return ImGui.GetCursorStartPos()                        end
M.GetCursorScreenPos =                function()               return ImGui.GetCursorScreenPos()                       end
M.GetCursorPos =                      function()               return ImGui.GetCursorPos()                             end
M.GetCursorPosX =                     function()               return ImGui.GetCursorPosX()                            end
M.GetCursorPosY =                     function()               return ImGui.GetCursorPosY()                            end
M.SetCursorPos =                      function(pos)            ImGui.SetCursorPos(pos)                                 end
M.SetCursorPosX =                     function(x)              ImGui.SetCursorPosX(x)                                  end
M.SetCursorPosY =                     function(y)              ImGui.SetCursorPosY(y)                                  end

M.GetIO =                             function()               return ImGui.GetIO()                                    end
M.GetStyle =                          function()               return ImGui.GetStyle()                                 end
--
M.CalcTextSize = function(text, text_end, hide_text_after_double_hash, wrap_width)
  text_end = nullableString(text_end)
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

M.InputFloat = function(label, value, step, stepFast, flags)
  step = step or 0
  stepFast = stepFast or 0
  flags = flags or 0
  return ImGui.InputFloat(label, value, step, stepFast, flags)
end

M.InputInt = function(label, value, step, stepFast, flags)
  step = step or 1
  stepFast = stepFast or 100
  flags = flags or 0
  return ImGui.InputInt(label, value, step, stepFast, flags)
end

M.InputText = function(label, text, flags)
  flags = flags or 0
  return ImGui.InputText(label, text, flags)
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

M.BeginPopupModal = function(name, open, flags)
  flags = flags or 0
  open = boolDefault(open, true)
  return ImGui.BeginPopupModal(name, open, flags)
end

M.EndPopup = function()               
  ImGui.EndPopup()                        
end

M.CloseCurrentPopup = function()               
  ImGui.CloseCurrentPopup()                        
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
  size = size or ImVec2(0, 0)
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

M.VSliderFloat = function(label, size, value, min, max, flags)
  min = min or FLOAT_SLIDER_MIN
  max = max or FLOAT_SLIDER_MAX
  flags = flags or 0
  return ImGui.VSliderFloat(label, size, value, min, max, flags)
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

M.SliderFloat = function(label, value, min, max, flags)
  min = min or FLOAT_SLIDER_MIN
  max = max or FLOAT_SLIDER_MAX
  flags = flags or 0
  return ImGui.SliderFloat(label, value, min, max, flags)
end

M.SliderFloat2 = function(label, x, y, min, max, flags)
  min = min or FLOAT_SLIDER_MIN
  max = max or FLOAT_SLIDER_MAX
  flags = flags or 0
  return ImGui.SliderFloat2(label, x, y, min, max, flags)
end

M.SliderFloat3 = function(label, x, y, z, min, max, flags)
  min = min or FLOAT_SLIDER_MIN
  max = max or FLOAT_SLIDER_MAX
  flags = flags or 0
  return ImGui.SliderFloat3(label, x, y, z, min, max, flags)
end

M.SliderFloat4 = function(label, x, y, z, w, min, max, flags)
  min = min or FLOAT_SLIDER_MIN
  max = max or FLOAT_SLIDER_MAX
  flags = flags or 0
  return ImGui.SliderFloat4(label, x, y, z, w, min, max, flags)
end

M.Slider = function(label, value, min, max)
  --Compatibility function, the name "Slider" was used
  --During development, but is now supposed to be SliderFloat
  return M.SliderFloat(label, value, min, max)
end

M.DragFloat4 = function(label, x, y, z, w, speed, min, max, flags)
  min = min or 0
  max = max or 0
  speed = speed or 1
  flags = flags or 0
  return ImGui.DragFloat4(label, x, y, z, w, speed, min, max, flags)
end

M.DragFloat3 = function(label, x, y, z, speed, min, max, flags)
  min = min or 0
  max = max or 0
  speed = speed or 1
  flags = flags or 0
  return ImGui.DragFloat3(label, x, y, z, speed, min, max, flags)
end

M.DragFloat2 = function(label, x, y, speed, min, max, flags)
  min = min or 0
  max = max or 0
  speed = speed or 1
  flags = flags or 0
  return ImGui.DragFloat2(label, x, y, speed, min, max, flags)
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
  size = size or ImVec2(0, 0)
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

M.DragFloat = function(label, value, speed, min, max, flags)
  min = min or 0
  max = max or 0
  speed = speed or 1
  flags = flags or 0
  return ImGui.DragFloat(label, value, speed, min, max, flags)
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
  -- arg2: either open status, or flags
  -- arg3: flags if open status is specified
  
  -- call Begin if we have all 3 args
  if type(arg2) == 'boolean' then
    arg3 = arg3 or 0
    return ImGui.Begin(name, arg2, arg3)
  elseif type(arg2) == 'number' or type(arg2) == 'nil' then
    arg2 = arg2 or 0
    return ImGui.Begin2(name, arg2)
  else
    print("imgui.Begin : invalid parameters (" .. name .. ", " .. tostring(arg2) .. ", " .. tostring(arg3) .. ")")
  end
end

M.InvisibleButton = function(text, size, flags)
  flags = flags or 0
  return ImGui.InvisibleButton(text, size, flags) 
end

M.Button = function(text, size)
  return ImGui.Button(text, size or ImVec2(0, 0))
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
  uv0 = uv0 or ImVec2(0, 0)
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
    
return M