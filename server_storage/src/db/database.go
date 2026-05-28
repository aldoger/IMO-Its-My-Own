package db

import (
	"fmt"
	"os"

	"gorm.io/driver/postgres"
	"gorm.io/gorm"
)

func New() *gorm.DB {
	DBHost := os.Getenv("DB_HOST")
	DBUser := os.Getenv("DB_USER")
	DBPassword := os.Getenv("DB_PASS")
	DBName := os.Getenv("DB_NAME")
	DBPort := os.Getenv("DB_PORT")

	if DBHost == "" || DBUser == "" || DBPassword == "" || DBName == "" || DBPort == "" {
		fmt.Println("Database environment variables are not properly set for development")
		os.Exit(1)
	}

	dbDSN := fmt.Sprintf(
		"host=%s user=%s password=%s dbname=%s port=%s sslmode=disable",
		DBHost, DBUser, DBPassword, DBName, DBPort,
	)

	db, err := gorm.Open(postgres.Open(dbDSN), &gorm.Config{})
	if err != nil {
		fmt.Println("Failed to connect to database ", err.Error())
		os.Exit(1)
	}

	fmt.Println("Database connected")
	return db
}
