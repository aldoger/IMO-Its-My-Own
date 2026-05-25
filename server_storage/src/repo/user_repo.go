package repo

import (
	"context"
	"imo-server-storage/src/model"

	"gorm.io/gorm"
)

type UserRepo struct {
	db *gorm.DB
}

func (ur *UserRepo) CreateUser(ctx context.Context, tx *gorm.DB, user model.User) (model.User, error) {
	if tx == nil {
		tx = ur.db
	}

	if err := tx.Create(user).Error; err != nil {
		return model.User{}, err
	}

	return user, nil
}
