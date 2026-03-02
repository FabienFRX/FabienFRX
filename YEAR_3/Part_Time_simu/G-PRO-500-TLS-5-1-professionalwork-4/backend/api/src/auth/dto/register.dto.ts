import { IsEmail, IsOptional, IsString, Length, Matches } from 'class-validator';

export class RegisterDto {
  @IsEmail() email: string;

  @IsString()
  @Matches(/^[a-zA-Z0-9_\.]+$/)
  @Length(3, 24)
  username: string;

  @IsString() @Length(8, 128)
  password: string;

  @IsOptional() @IsString()
  displayName?: string;
}
