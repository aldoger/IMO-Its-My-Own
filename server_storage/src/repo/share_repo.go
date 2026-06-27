package repo

import (
	"context"
	"imo-server-storage/src/model"

	"github.com/google/uuid"
	"gorm.io/gorm"
)

type ShareRepository struct {
	db *gorm.DB
}

func NewShareRepository(db *gorm.DB) ShareRepository {
	return ShareRepository{db: db}
}

func (r *ShareRepository) Create(ctx context.Context, tx *gorm.DB, share model.Share) error {

	if tx == nil {
		tx = r.db
	}

	if err := tx.WithContext(ctx).Create(&share).Error; err != nil {
		return err
	}

	return nil
}

func (r *ShareRepository) Remove(ctx context.Context, tx *gorm.DB, fileId uuid.UUID, sharedToId uuid.UUID) error {

	if tx == nil {
		tx = r.db
	}

	if err := tx.WithContext(ctx).Delete(model.Share{}, "file_id = ? AND shared_to_id = ?", fileId, sharedToId).Error; err != nil {
		return err
	}

	return nil
}

func (r *ShareRepository) Delete(ctx context.Context, tx *gorm.DB, fileId uuid.UUID) error {

	if tx == nil {
		tx = r.db
	}

	if err := tx.WithContext(ctx).Delete(model.Share{}, "file_id = ?", fileId).Error; err != nil {
		return err
	}

	return nil
}
