import { IsEmail, IsOptional, IsString, Length, Matches } from 'class-validator';

export class CreateUserDto {
  @IsEmail() email: string;
  @IsString()
  @Matches(/^[a-zA-Z0-9_\.]+$/)
  @Length(3, 24)
  username: string;
  @IsOptional() @IsString() displayName?: string;
  @IsOptional() @IsString() avatarUrl?: string;
}
