package model

import (
	"github.com/google/uuid"
	"gorm.io/gorm"
)

type User struct {
	ID       uuid.UUID `gorm:"type:uuid;primaryKey" json:"id"`
	Username string    `gorm:"type:varchar(100);not null;unique" json:"username"`

	Files       []File  `gorm:"foreignKey:OwnerID;references:ID"`
	SharedFiles []Share `gorm:"foreignKey:SharedID;references:ID"`
}

func (u *User) BeforeCreate(tx *gorm.DB) (err error) {
	u.ID = uuid.New()
	return
}

func NewUser(username string) User {
	return User{Username: username}
}
