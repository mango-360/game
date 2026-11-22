-- Creates the game database and its tables.
-- Run once from SSMS (or: sqlcmd -S localhost\SQLEXPRESS -E -C -i run\sql\create_test_db.sql)

IF DB_ID(N'PlatformerTestDB') IS NULL
	CREATE DATABASE PlatformerTestDB;
GO

USE PlatformerTestDB;
GO

-- Used by the test button on the title screen
IF OBJECT_ID(N'dbo.TestData', N'U') IS NULL
BEGIN
	CREATE TABLE dbo.TestData
	(
		Id INT IDENTITY(1,1) PRIMARY KEY,
		PlayerName NVARCHAR(50) NOT NULL,
		Score INT NOT NULL,
		CreatedAt DATETIME2 NOT NULL DEFAULT SYSDATETIME()
	);
END
GO

-- All item types that can drop. Ids match the TYLE_DROP enum in defines.h
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
		(4, N'Stick');
END
GO

-- How many of each item the player has picked up
IF OBJECT_ID(N'dbo.Inventory', N'U') IS NULL
BEGIN
	CREATE TABLE dbo.Inventory
	(
		ItemId INT PRIMARY KEY REFERENCES dbo.Items(Id),
		Quantity INT NOT NULL DEFAULT 0
	);
END
GO
