import { IsArray, ArrayMaxSize, IsBoolean, IsEnum, IsInt, IsOptional, IsString, Length, Min, Max } from 'class-validator';
import { AgeGroup, Level } from '@prisma/client';

export class UpdateUserDto {
  @IsOptional() @IsString() @Length(3, 24) displayName?: string;
  @IsOptional() @IsString() avatarUrl?: string;
  @IsOptional() @IsString() country?: string;
  @IsOptional() @IsInt() @Min(1900) @Max(new Date().getFullYear()) birthYear?: number;
  @IsOptional() @IsEnum(AgeGroup) ageGroup?: AgeGroup;
  @IsOptional() @IsEnum(Level) defaultLevel?: Level;
  @IsOptional() @IsArray() @ArrayMaxSize(2) favoriteTopics?: string[];
  @IsOptional() @IsBoolean() trickyAnswersEnabled?: boolean;
}
