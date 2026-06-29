package repo

import (
	"context"
	"imo-server-storage/src/model"

	"gorm.io/gorm"
)

type UserRepo struct {
	db *gorm.DB
}

func NewUserRepo(db *gorm.DB) UserRepo {
	return UserRepo{db: db}
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

func (r *UserRepo) FindUserByUserName(ctx context.Context, tx *gorm.DB, userName string) (string, error) {
	if tx == nil {
		tx = r.db
	}

	var user model.User
	if err := tx.WithContext(ctx).Select("id").First(&user, "username = ?", userName).Error; err != nil {
		return "", err
	}

	return user.ID.String(), nil
}
