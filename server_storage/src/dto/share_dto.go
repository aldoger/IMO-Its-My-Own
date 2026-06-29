package dto

type CreateShareRequest struct {
	Filename       string `json:"file_name"`
	TargetUserName string `json:"target_user_name"`
}

type RemoveShareRequest struct {
	Filename       string `json:"file_name"`
	TargetUserName string `json:"target_user_name"`
}
