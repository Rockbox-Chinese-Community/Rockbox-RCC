--[[
             __________               __   ___.
   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
                     \/            \/     \/    \/            \/
 $Id$

 Port of Chain Reaction (which is based on Boomshine) to Rockbox in Lua.
 See http://www.yvoschaap.com/chainrxn/ and http://www.k2xl.com/games/boomshine/

 Copyright (C) 2009 by Maurus Cuelenaere

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 KIND, either express or implied.

]]--

require "actions"

local CYCLETIME = rb.HZ / 50
local HAS_TOUCHSCREEN = rb.action_get_touchscreen_press ~= nil
local DEFAULT_BALL_SIZE = rb.LCD_HEIGHT > rb.LCD_WIDTH and rb.LCD_WIDTH  / 30
                                                       or  rb.LCD_HEIGHT / 30
local MAX_BALL_SPEED = DEFAULT_BALL_SIZE / 2
local DEFAULT_FOREGROUND_COLOR = rb.lcd_get_foreground ~= nil
                                                     and rb.lcd_get_foreground()
                                                     or  0

local levels = {
            --  {GOAL, TOTAL_BALLS},
                {1,   5},
                {2,  10},
                {4,  15},
                {6,  20},
                {10, 25},
                {15, 30},
                {18, 35},
                {22, 40},
                {30, 45},
                {37, 50},
                {48, 55},
                {55, 60}
           }

local Ball = {
                size = DEFAULT_BALL_SIZE,
                exploded = false,
                implosion = false
             }

function Ball:new(o)
    if o == nil then
        o = {
                x = math.random(0, rb.LCD_WIDTH - self.size),
                y = math.random(0, rb.LCD_HEIGHT - self.size),
                color = random_color(),
                up_speed = Ball:generateSpeed(),
                right_speed = Ball:generateSpeed(),
                explosion_size = math.random(2*self.size, 4*self.size),
                life_duration = math.random(rb.HZ, rb.HZ*5)
            }
    end

    setmetatable(o, self)
    self.__index = self
    return o
end

function Ball:generateSpeed()
    local speed = math.random(-MAX_BALL_SPEED, MAX_BALL_SPEED)
    if speed == 0 then
        speed = 1       -- Make sure all balls move
    end

    return speed
end

function Ball:draw()
    --[[
         I know these aren't circles, but as there's no current circle
         implementation in Rockbox, rectangles will just do fine (drawing
         circles from within Lua is far too slow).
    ]]--
    set_foreground(self.color)
    rb.lcd_fillrect(self.x, self.y, self.size, self.size)
end

function Ball:step()
    if self.exploded then
        if self.implosion and self.size > 0 then
            self.size = self.size - 2
            self.x = self.x + 1 -- We do this because we want to stay centered
            self.y = self.y + 1
        elseif self.size < self.explosion_size then
            self.size = self.size + 2
            self.x = self.x - 1 -- We do this for the same reasons as above
            self.y = self.y - 1
        end
        return
    end

    self.x = self.x + self.right_speed
    self.y = self.y + self.up_speed
    if (self.right_speed > 0 and self.x + self.size >= rb.LCD_WIDTH) or
       (self.right_speed < 0 and self.x <= 0) then
        self.right_speed = -self.right_speed
    end
    if (self.up_speed > 0 and self.y + self.size >= rb.LCD_HEIGHT) or
       (self.up_speed < 0 and self.y <= 0) then
        self.up_speed = -self.up_speed
    end
end

function Ball:checkHit(other)
    if (other.x + other.size >= self.x) and (self.x + self.size >= other.x) and
       (other.y + other.size >= self.y) and (self.y + self.size >= other.y) then
        assert(not self.exploded)
        self.exploded = true
        self.death_time = rb.current_tick() + self.life_duration
        if not other.exploded then
            other.exploded = true
            other.death_time = rb.current_tick() + other.life_duration
        end
        return true
    end

    return false
end

local Cursor = {
                size = DEFAULT_BALL_SIZE*2,
                x = rb.LCD_WIDTH/2,
                y = rb.LCD_HEIGHT/2
             }

function Cursor:new()
    return self
end

function Cursor:do_action(action)
    if action == rb.actions.ACTION_TOUCHSCREEN and HAS_TOUCHSCREEN then
        _, self.x, self.y = rb.action_get_touchscreen_press()
        return true
    elseif action == rb.actions.PLA_SELECT then
        return true
    elseif (action == rb.actions.PLA_RIGHT) then
        self.x = self.x + self.size
    elseif (action == rb.actions.PLA_LEFT) then
        self.x = self.x - self.size
    elseif (action == rb.actions.PLA_UP) then
        self.y = self.y - self.size
    elseif (action == rb.actions.PLA_DOWN) then
        self.y = self.y + self.size
    end

    if self.x > rb.LCD_WIDTH then
        self.x = 0
    elseif self.x < 0 then
        self.x = rb.LCD_WIDTH
    end

    if self.y > rb.LCD_HEIGHT then
        self.y = 0
    elseif self.y < 0 then
        self.y = rb.LCD_HEIGHT
    end

    return false
end

function Cursor:draw()
    set_foreground(DEFAULT_FOREGROUND_COLOR)

    rb.lcd_hline(self.x - self.size/2, self.x - self.size/4, self.y - self.size/2)
    rb.lcd_hline(self.x + self.size/4, self.x + self.size/2, self.y - self.size/2)
    rb.lcd_hline(self.x - self.size/2, self.x - self.size/4, self.y + self.size/2)
    rb.lcd_hline(self.x + self.size/4, self.x + self.size/2, self.y + self.size/2)
    rb.lcd_vline(self.x - self.size/2, self.y - self.size/2, self.y - self.size/4)
    rb.lcd_vline(self.x - self.size/2, self.y + self.size/4, self.y + self.size/2)
    rb.lcd_vline(self.x + self.size/2, self.y - self.size/2, self.y - self.size/4)
    rb.lcd_vline(self.x + self.size/2, self.y + self.size/4, self.y + self.size/2)

    rb.lcd_hline(self.x - self.size/4, self.x + self.size/4, self.y)
    rb.lcd_vline(self.x, self.y - self.size/4, self.y + self.size/4)
end

function draw_positioned_string(bottom, right, str)
    local _, w, h = rb.font_getstringsize(str, rb.FONT_UI)

    rb.lcd_putsxy((rb.LCD_WIDTH-w)*right, (rb.LCD_HEIGHT-h)*bottom, str)
end

function set_foreground(color)
    if rb.lcd_set_foreground ~= nil then
        rb.lcd_set_foreground(color)
    end
end

function random_color()
    if rb.lcd_rgbpack ~= nil then --color target
        return rb.lcd_rgbpack(math.random(1,255), math.random(1,255), math.random(1,255))
    end

    return math.random(1, rb.LCD_DEPTH)
end

function start_round(level, goal, nrBalls, total)
    local player_added, score, exit, nrExpandedBalls = false, 0, false, 0
    local balls, explodedBalls = {}, {}
    local cursor = Cursor:new()

    -- Initialize the balls
    for _=1,nrBalls do
        table.insert(balls, Ball:new())
    end

    -- Make sure there are no unwanted touchscreen presses
    rb.button_clear_queue()

    while true do
        local endtick = rb.current_tick() + CYCLETIME

        -- Check if the round is over
        if #explodedBalls == 0 and player_added then
            break
        end

        -- Check for actions
        local action = rb.get_plugin_action(0)
        if action == rb.actions.PLA_EXIT or action == rb.actions.PLA_CANCEL then
            exit = true
            break
        end
        if not player_added and cursor:do_action(action) then
            local player = Ball:new({
                                x = cursor.x,
                                y = cursor.y,
                                color = DEFAULT_FOREGROUND_COLOR,
                                size = 10,
                                explosion_size = 3*DEFAULT_BALL_SIZE,
                                exploded = true,
                                death_time = rb.current_tick() + rb.HZ * 3
                            })
            table.insert(explodedBalls, player)
            player_added = true
        end

        -- Check for hits
        for i, ball in ipairs(balls) do
            for _, explodedBall in ipairs(explodedBalls) do
                if ball:checkHit(explodedBall) then
                    score = score + 100*level
                    nrExpandedBalls = nrExpandedBalls + 1
                    table.insert(explodedBalls, ball)
                    table.remove(balls, i)
                    break
                end
            end
        end

        -- Check if we're dead yet
        for i, explodedBall in ipairs(explodedBalls) do
            if rb.current_tick() >= explodedBall.death_time then
                if explodedBall.size > 0 then
                    explodedBall.implosion = true -- We should be dying
                else
                    table.remove(explodedBalls, i) -- We're imploded!
                end
            end
        end

        -- Drawing phase
        rb.lcd_clear_display()

        set_foreground(DEFAULT_FOREGROUND_COLOR)
        draw_positioned_string(0, 0, string.format("%d balls expanded", nrExpandedBalls))
        draw_positioned_string(0, 1, string.format("Level %d", level))
        draw_positioned_string(1, 1, string.format("%d level points", score))
        draw_positioned_string(1, 0, string.format("%d total points", total+score))

        for _, ball in ipairs(balls) do
            ball:step()
            ball:draw()
        end

        for _, explodedBall in ipairs(explodedBalls) do
            explodedBall:step()
            explodedBall:draw()
        end

        if not HAS_TOUCHSCREEN and not player_added then
            cursor:draw()
        end

        -- Push framebuffer to the LCD
        rb.lcd_update()

        if rb.current_tick() < endtick then
            rb.sleep(endtick - rb.current_tick())
        else
            rb.yield()
        end
    end

    return exit, score, nrExpandedBalls
end

-- Helper function to display a message
function display_message(to, ...)
    local message = string.format(...)
    local _, w, h = rb.font_getstringsize(message, rb.FONT_UI)
    local x, y = (rb.LCD_WIDTH - w) / 2, (rb.LCD_HEIGHT - h) / 2

    rb.lcd_clear_display()
    set_foreground(DEFAULT_FOREGROUND_COLOR)
    if w > rb.LCD_WIDTH then
        rb.lcd_puts_scroll(0, y/h, message)
    else
        rb.lcd_putsxy(x, y, message)
    end
    if to == -1 then
        local msg = "Press button to exit"
        w = rb.font_getstringsize(msg, rb.FONT_UI)
        x = (rb.LCD_WIDTH - w) / 2
        if x < 0 then
            rb.lcd_puts_scroll(0, y/h + 1, msg)
        else
            rb.lcd_putsxy(x, y + h, msg)
        end
    end
    rb.lcd_update()

    if to == -1 then
        rb.sleep(rb.HZ/2)
        rb.button_clear_queue()
        rb.button_get(1)
    else
        rb.sleep(to)
    end

    rb.lcd_scroll_stop() -- Stop our scrolling message
end

if HAS_TOUCHSCREEN then
    rb.touchscreen_set_mode(rb.TOUCHSCREEN_POINT)
end
rb.backlight_force_on()

local idx, highscore = 1, 0
while levels[idx] ~= nil do
    local goal, nrBalls = levels[idx][1], levels[idx][2]

    display_message(rb.HZ*2, "Level %d: get %d out of %d balls", idx, goal, nrBalls)

    local exit, score, nrExpandedBalls = start_round(idx, goal, nrBalls, highscore)
    if exit then
        break -- Exiting..
    else
        if nrExpandedBalls >= goal then
            display_message(rb.HZ*2, "You won!")
            idx = idx + 1
            highscore = highscore + score
        else
            display_message(rb.HZ*2, "You lost!")
        end
    end
end

if idx > #levels then
    display_message(-1, "You finished the game with %d points!", highscore)
else
    display_message(-1, "You made it till level %d with %d points!", idx, highscore)
end

-- Restore user backlight settings
rb.backlight_use_settings()
