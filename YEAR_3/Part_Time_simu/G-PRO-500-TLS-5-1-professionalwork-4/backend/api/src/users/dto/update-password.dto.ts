import { IsString, Length } from 'class-validator';

export class UpdatePasswordDto {
  @IsString() currentPassword: string;
  @IsString() @Length(8, 128) newPassword: string;
}
