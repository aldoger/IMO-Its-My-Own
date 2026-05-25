package dto

type CreateUserRequest struct {
	Username string `json:"username"`
}

type CreateUserResponse struct {
	ID string `json:"id"`
}
