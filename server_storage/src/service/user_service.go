package service

import (
	"context"
	"imo-server-storage/src/dto"
	"imo-server-storage/src/model"
	"imo-server-storage/src/repo"
)

type UserService struct {
	UserRepo repo.UserRepo
}

func (us *UserService) CreateUser(ctx context.Context, userData dto.CreateUserRequest) (dto.CreateUserResponse, error) {
	newUser := model.NewUser(userData.Username)

	userID, err := us.UserRepo.CreateUser(ctx, nil, newUser)
	if err != nil {
		return dto.CreateUserResponse{}, err
	}

	return dto.CreateUserResponse{
		ID: userID.ID.String(),
	}, nil
}

func NewUserService(userRepo repo.UserRepo) UserService {
	return UserService{UserRepo: userRepo}
}
