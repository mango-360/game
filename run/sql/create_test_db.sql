-- Creates the game database and its tables.
-- Run once from SSMS (or: sqlcmd -S localhost\SQLEXPRESS -E -C -i run\sql\create_test_db.sql)

IF DB_ID(N'PlatformerTestDB') IS NULL
	CREATE DATABASE PlatformerTestDB;
GO

USE PlatformerTestDB;
GO

-- One row per player profile
IF OBJECT_ID(N'dbo.Players', N'U') IS NULL
BEGIN
	CREATE TABLE dbo.Players
	(
		Id INT IDENTITY(1,1) PRIMARY KEY,
		Name NVARCHAR(50) NOT NULL UNIQUE,
		CreatedAt DATETIME2 NOT NULL DEFAULT SYSDATETIME()
	);
END
GO

-- Columns for log in, auto log in and skins (added separately so older databases get them too)
IF COL_LENGTH(N'dbo.Players', N'PasswordHash') IS NULL
	ALTER TABLE dbo.Players ADD PasswordHash VARBINARY(32) NULL; -- SHA-256 of "name:password"
GO

IF COL_LENGTH(N'dbo.Players', N'SessionToken') IS NULL
	ALTER TABLE dbo.Players ADD SessionToken UNIQUEIDENTIFIER NULL; -- saved in config\session.txt
GO

IF COL_LENGTH(N'dbo.Players', N'Skin') IS NULL
	ALTER TABLE dbo.Players ADD Skin INT NOT NULL DEFAULT 0; -- index in SKINS (Account.h)
GO

-- All item types that can drop. Ids match the DROP_TYPE enum in defines.h
IF OBJECT_ID(N'dbo.Items', N'U') IS NULL
BEGIN
	CREATE TABLE dbo.Items
	(
		Id INT PRIMARY KEY,
		Name NVARCHAR(50) NOT NULL
	);

	INSERT INTO dbo.Items (Id, Name) VALUES
		(2, N'Grass block'),
		(3, N'Dirt block'),
		(4, N'Stick'),
		(5, N'Leaf');
END
GO

-- How many of each item every player has picked up
IF OBJECT_ID(N'dbo.Inventory', N'U') IS NULL
BEGIN
	CREATE TABLE dbo.Inventory
	(
		PlayerId INT NOT NULL REFERENCES dbo.Players(Id),
		ItemId INT NOT NULL REFERENCES dbo.Items(Id),
		Quantity INT NOT NULL DEFAULT 0,
		PRIMARY KEY (PlayerId, ItemId)
	);
END
GO
