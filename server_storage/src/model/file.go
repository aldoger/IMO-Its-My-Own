package model

import (
	"github.com/google/uuid"
	"gorm.io/gorm"
)

type File struct {
	ID       uuid.UUID `gorm:"type:uuid;primaryKey" json:"id"`
	OwnerID  uuid.UUID `json:"owner_id"`
	Filename string    `json:"filename"`
	Size     uint      `json:"size"`
}

func (u *File) BeforeCreate(tx *gorm.DB) (err error) {
	u.ID = uuid.New()
	return
}

func (u *File) ChangeOwner(ownerID uuid.UUID) {
	u.OwnerID = ownerID
}
