/* cs_serveur.cpp */

SET @id = 718;

-- Add new permissions
DELETE FROM `rbac_permissions` WHERE `id` BETWEEN @id AND @id+18;
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
(@id+0, 'serveur'),
(@id+1, 'serveur corpses'),
(@id+2, 'serveur exit'),
(@id+3, 'serveur idlerestart'),
(@id+4, 'serveur idlerestart cancel'),
(@id+5, 'serveur idleshutdown'),
(@id+6, 'serveur idleshutdown cancel'),
(@id+7, 'serveur info'),
(@id+8, 'serveur plimit'),
(@id+9, 'serveur restart'),
(@id+10, 'serveur restart cancel'),
(@id+11, 'serveur set'),
(@id+12, 'serveur set closed'),
(@id+13, 'serveur set difftime'),
(@id+14, 'serveur set loglevel'),
(@id+15, 'serveur set motd'),
(@id+16, 'serveur shutdown'),
(@id+17, 'serveur shutdown cancel'),
(@id+18, 'serveur motd');

-- Add permissions to "corresponding Commands Role"
DELETE FROM `rbac_role_permissions` WHERE `permissionId` BETWEEN @id AND @id+18;
INSERT INTO `rbac_role_permissions` (`roleId`, `permissionId`) VALUES
(4, @id+0),
(4, @id+1),
(4, @id+3),
(4, @id+4),
(4, @id+5),
(4, @id+6),
(4, @id+7),
(4, @id+8),
(4, @id+9),
(4, @id+10),
(4, @id+11),
(4, @id+12),
(4, @id+15),
(4, @id+16),
(4, @id+17),
(4, @id+18);
