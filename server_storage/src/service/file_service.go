package service

import (
	"context"
	"imo-server-storage/src/dto"
	"imo-server-storage/src/model"
	"imo-server-storage/src/repo"
	"imo-server-storage/src/utils"

	"github.com/google/uuid"
	"golang.org/x/sync/errgroup"
)

type FileService struct {
	fileRepo *repo.FileRepo
}

func NewFileService(fileRepo *repo.FileRepo) FileService {
	return FileService{fileRepo: fileRepo}
}

func (s *FileService) Upload(ctx context.Context, fileData *dto.UploadFileRequest) error {

	file := model.NewFile(
		uuid.MustParse(fileData.UserID),
		fileData.FileName,
		0,
	)

	g, ctx := errgroup.WithContext(ctx)

	g.Go(func() error {
		return utils.UploadFile(ctx, *fileData.File, fileData.FileName)
	})

	g.Go(func() error {
		return s.fileRepo.AddFile(ctx, nil, file)
	})

	return g.Wait()
}
