yage.set_music("music/penso_positivo.wav")
yage.set_video("videos/clouds.mp4")

local enemies = {
    {"SeekeR", 40, 300, 0}, {"seeker", 40, 400, 0}, {"simple", 65, 20, 0},
    {"simple", 40, 300, 10}, {"simple", 40, 400, 20}, {"simple", 65, 20, 20},
}

for _, ene in ipairs(enemies) do
    yage.add_enemy(ene[1], ene[2], ene[3], ene[4])
end


yage.add_text([[
This is the second level.
Please eliminate all the letters 
on screen!
]])

-- music = "music/penso_positivo.wav"
-- video = "videos/clouds.mp4"

-- enemy_config = {
--     seeker = {
--         collider = {20, 20},
--         sprite = {20, 20},
--         color = {0, 0, 256, 256},
--         health = 5,
--         weapon_power = 5
--         -- on_update = {}
--     }
-- }

-- enemies = {
--     {"seeker", 40, 300, 0}, {"seeker", 40, 400, 0}, {"simple", 65, 20, 0},
--     {"simple", 300, 500, 0}, {"simple", 65, 60, 0}, {"simple", 125, 80, 0},
--     {"seeker", 500, 400, 0}, {"seeker", 200, 400, 0}, {"seeker", 300, 400, 0},
--     {"simple", 120, 100, 0}, {"simple", 125, 200, 0}, {"simple", 65, 120, 0},
--     {"simple", 60, 140, 0}, {"simple", 65, 160, 0}, {"simple", 605, 120, 0},
--     {"simple", 600, 140, 0}, {"simple", 605, 160, 0}
-- }
