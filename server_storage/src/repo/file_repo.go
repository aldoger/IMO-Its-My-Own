package repo

import (
	"context"
	"imo-server-storage/src/model"

	"github.com/google/uuid"
	"gorm.io/gorm"
)

type FileRepo struct {
	db *gorm.DB
}

func NewFileRepo(db *gorm.DB) FileRepo {
	return FileRepo{db: db}
}

func (r *FileRepo) AddFile(ctx context.Context, tx *gorm.DB, file model.File) error {
	if tx == nil {
		tx = r.db
	}

	if err := tx.WithContext(ctx).Create(&file).Error; err != nil {
		return err
	}

	return nil
}

func (r *FileRepo) Delete(ctx context.Context, tx *gorm.DB, fileId uuid.UUID) error {
	if tx == nil {
		tx = r.db
	}

	if err := tx.WithContext(ctx).Delete(&model.File{}, "id = ?", fileId).Error; err != nil {
		return err
	}

	return nil
}
