package model

import (
	"github.com/google/uuid"
	"gorm.io/gorm"
)

type Share struct {
	ID uuid.UUID `gorm:"type:uuid;primaryKey"`

	FileID     uuid.UUID `json:"file_id"`
	SharedToID uuid.UUID `json:"shared_to_id"`

	File     File `gorm:"foreignKey:FileID"`
	SharedTo User `gorm:"foreignKey:SharedToID"`
}

func (s *Share) BeforeCreate(tx *gorm.DB) (err error) {
	s.ID = uuid.New()
	return
}

func NewShare(fileId, targetUserId uuid.UUID) Share {
	return Share{
		FileID:     fileId,
		SharedToID: targetUserId,
	}
}
