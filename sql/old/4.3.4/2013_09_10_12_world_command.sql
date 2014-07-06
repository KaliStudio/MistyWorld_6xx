/* cs_serveur.cpp */

SET @id = 718;

-- Update command table with new RBAC permissions
UPDATE `command` SET `permission` = @id+0 WHERE `name` = 'serveur';
UPDATE `command` SET `permission` = @id+1 WHERE `name` = 'serveur corpses';
UPDATE `command` SET `permission` = @id+2 WHERE `name` = 'serveur exit';
UPDATE `command` SET `permission` = @id+3 WHERE `name` = 'serveur idlerestart';
UPDATE `command` SET `permission` = @id+4 WHERE `name` = 'serveur idlerestart cancel';
UPDATE `command` SET `permission` = @id+5 WHERE `name` = 'serveur idleshutdown';
UPDATE `command` SET `permission` = @id+6 WHERE `name` = 'serveur idleshutdown cancel';
UPDATE `command` SET `permission` = @id+7 WHERE `name` = 'serveur info';
UPDATE `command` SET `permission` = @id+8 WHERE `name` = 'serveur plimit';
UPDATE `command` SET `permission` = @id+9 WHERE `name` = 'serveur restart';
UPDATE `command` SET `permission` = @id+10 WHERE `name` = 'serveur restart cancel';
UPDATE `command` SET `permission` = @id+11 WHERE `name` = 'serveur set';
UPDATE `command` SET `permission` = @id+12 WHERE `name` = 'serveur set closed';
UPDATE `command` SET `permission` = @id+13 WHERE `name` = 'serveur set difftime';
UPDATE `command` SET `permission` = @id+14 WHERE `name` = 'serveur set loglevel';
UPDATE `command` SET `permission` = @id+15 WHERE `name` = 'serveur set motd';
UPDATE `command` SET `permission` = @id+16 WHERE `name` = 'serveur shutdown';
UPDATE `command` SET `permission` = @id+17 WHERE `name` = 'serveur shutdown cancel';
UPDATE `command` SET `permission` = @id+18 WHERE `name` = 'serveur motd';
