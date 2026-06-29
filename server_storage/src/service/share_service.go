package service

import (
	"context"
	"imo-server-storage/src/dto"
	"imo-server-storage/src/model"
	"imo-server-storage/src/repo"

	"github.com/google/uuid"
)

type ShareService struct {
	shareRepo *repo.ShareRepository
	fileRepo  *repo.FileRepo
	userRepo  *repo.UserRepo
}

func NewShareService(shareRepo *repo.ShareRepository) ShareService {
	return ShareService{shareRepo: shareRepo}
}

func (s *ShareService) CreateShare(ctx context.Context, shareData dto.CreateShareRequest) error {

	var userId, fileId string
	var chUser = make(chan string)
	var chFile = make(chan string)

	chUserErr := make(chan error)
	chFileErr := make(chan error)

	go func() {
		id, err := s.userRepo.FindUserByUserName(ctx, nil, shareData.TargetUserName)
		if err != nil {
			chUserErr <- err
			return
		}
		chUser <- id
	}()

	go func() {
		id, err := s.fileRepo.GetFileByName(ctx, nil, shareData.Filename)
		if err != nil {
			chFileErr <- err
			return
		}
		chFile <- id
	}()

	for i := 0; i < 2; i++ {
		select {
		case id := <-chUser:
			userId = id
		case id := <-chFile:
			fileId = id
		case err := <-chUserErr:
			return err
		case err := <-chFileErr:
			return err
		}
	}

	uuidFile := uuid.MustParse(fileId)
	uuidUser := uuid.MustParse(userId)

	newShare := model.NewShare(uuidFile, uuidUser)

	if err := s.shareRepo.Create(ctx, nil, newShare); err != nil {
		return err
	}

	return nil
}
